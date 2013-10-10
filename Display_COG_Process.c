// Copyright 2013 Pervasive Displays, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at:
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
// express or implied. See the License for the specific language
// governing permissions and limitations under the License.

/******************************************************************************
 * Includes
 *****************************************************************************/
//#include "driver_config.h"
//#include "target_config.h"
//#include "type.h"
//#include "gpio.h"
#include "Display_COG_Process.h"
#include "Display_Hardware_Driver.h"
#include "Display_Controller.h"


/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/
#define _GaugeFrameTimeMark		0

const COG_Parameters_t COG_Parameters[3]= {
	{	//for 1.44"
		{0x00,0x00,0x00,0x00,0x00,0x0f,0xff,0x00},
		0x03,
		(128/8),
		96,
		(((128+96)*2)/8),
		(43),
		480
	},
	{	//for 2.0"
		{0x00,0x00,0x00,0x00,0x01,0xFF,0xE0,0x00},
		0x03,
		(200/8),
		96,
		(((200+96)*2)/8)+1,
		(53),
		480
	},
	{	//for 2.7"
		{0x00,0x00,0x00,0x7F,0xFF,0xFE,0x00,0x00},
		0x00,
		(264/8),
		176,
		(((264+176)*2)/8)+1,
		105,
		630
	},
};

const uint8_t ScanTable[4] = {0xC0, 0x30, 0x0C, 0x03};


/******************************************************************************
 * Local variables
 *****************************************************************************/
static uint32_t	StageTime = 480;
static COG_LineDataPacket_t COG_Line;
static uint16_t	EPD_Type_Index = 0;
static uint16_t	cnt = 0;
static uint32_t	Currentframe;
static uint8_t	*DataLineEven;
static uint8_t	*DataLineOdd;
static uint8_t	*DataLineScan;
static uint8_t	*DisplayOrgPtr;


/******************************************************************************
 * Local functions
 *****************************************************************************/
static void SetTemperature_Factor(uint8_t EPD_Type_Index)
{
	int16_t Temperature;

	//Get current temperature
	Temperature = epd_get_temperature();

	if (Temperature < -10)
	{
		StageTime = COG_Parameters[EPD_Type_Index].StageTime * 17;
	}
	else if (Temperature < -5)
	{
		StageTime = COG_Parameters[EPD_Type_Index].StageTime * 12;
	}
	else if (Temperature < 5)
	{
		StageTime = COG_Parameters[EPD_Type_Index].StageTime * 8;
	}
	else if (Temperature < 10)
	{
		StageTime = COG_Parameters[EPD_Type_Index].StageTime * 4;
	}
	else if (Temperature < 15)
	{
		StageTime = COG_Parameters[EPD_Type_Index].StageTime * 3;
	}
	else if (Temperature < 20)
	{
		StageTime = COG_Parameters[EPD_Type_Index].StageTime * 2;
	}
	else if (Temperature < 40)
	{
		StageTime = COG_Parameters[EPD_Type_Index].StageTime * 1;
	}
	else
	{
		StageTime = (COG_Parameters[EPD_Type_Index].StageTime * 7)/10;
	}
}

static void Driver_TypeSelect(uint8_t typeIndex)
{
	switch(typeIndex)
	{
		case EPDType_144:
			DataLineEven=&COG_Line.LineDatas.COG_144LineData.Even[0];
			DataLineOdd=&COG_Line.LineDatas.COG_144LineData.Odd[0];
			DataLineScan=&COG_Line.LineDatas.COG_144LineData.Scan[0];
			break;
		case EPDType_200:
			DataLineEven=&COG_Line.LineDatas.COG_20LineData.Even[0];
			DataLineOdd=&COG_Line.LineDatas.COG_20LineData.Odd[0];
			DataLineScan=&COG_Line.LineDatas.COG_20LineData.Scan[0];
			break;
		case EPDType_270:
			DataLineEven=&COG_Line.LineDatas.COG_27LineData.Even[0];
			DataLineOdd=&COG_Line.LineDatas.COG_27LineData.Odd[0];
			DataLineScan=&COG_Line.LineDatas.COG_27LineData.Scan[0];
			break;
	}
}

//#pragma GCC optimize ("-O3")
static void Display_Stage_1 (uint8_t *PreviousPicture)
{
	uint16_t	i,j,k;					// general counters
	uint8_t		TempByte;				// Temporary storage for image data check
	uint16_t	StartClock;
#ifdef DISPLAY_180_DEGREE
	uint8_t     *DataLinePrt;
    DisplayOrgPtr = (PreviousPicture+(uint32_t)((COG_Parameters[EPD_Type_Index].VERTICAL-1)*COG_Parameters[EPD_Type_Index].HORIZONTAL));
#else
    DisplayOrgPtr = PreviousPicture;
#endif
	Currentframe = (uint32_t)COG_Parameters[EPD_Type_Index].FrameTime;
//TestPinTrigger();
	cnt = 0;
	StartClock = epd_GetCurrentTimeTick();
	do
	{	   	  
//	    TestPin2Trigger();
	    PreviousPicture = DisplayOrgPtr;
#ifdef DISPLAY_180_DEGREE
		DataLinePrt = PreviousPicture;
#endif
		for (i = 0; i < COG_Parameters[EPD_Type_Index].VERTICAL; i++)			// for every line
		{
			// SPI (0x04, ...)
	        epd_SPI_Send_Byte(0x04, COG_Parameters[EPD_Type_Index].VoltageLevel);
	        k = COG_Parameters[EPD_Type_Index].HORIZONTAL-1;
			for (j = 0; j < COG_Parameters[EPD_Type_Index].HORIZONTAL; j++)		// check every bit in the line
			{			
				TempByte = (*PreviousPicture++);							
#ifdef DISPLAY_180_DEGREE
				DataLineEven[j]	 =    ((TempByte & 0x80) ? BLACK3  : WHITE3)
									| ((TempByte & 0x20) ? BLACK2  : WHITE2)
									| ((TempByte & 0x08) ? BLACK1  : WHITE1)
									| ((TempByte & 0x02) ? BLACK0  : WHITE0);
			
				DataLineOdd[k--] =    ((TempByte & 0x01) ? BLACK3  : WHITE3)
									| ((TempByte & 0x04) ? BLACK2  : WHITE2)
									| ((TempByte & 0x10) ? BLACK1  : WHITE1)
									| ((TempByte & 0x40) ? BLACK0  : WHITE0);
			    DataLinePrt--;
#else
				DataLineOdd[j] =      ((TempByte & 0x80) ? BLACK3  : WHITE3)
									| ((TempByte & 0x20) ? BLACK2  : WHITE2)
									| ((TempByte & 0x08) ? BLACK1  : WHITE1)
									| ((TempByte & 0x02) ? BLACK0  : WHITE0);

				DataLineEven[k--] =   ((TempByte & 0x01) ? BLACK3  : WHITE3)
									| ((TempByte & 0x04) ? BLACK2  : WHITE2)
									| ((TempByte & 0x10) ? BLACK1  : WHITE1)
									| ((TempByte & 0x40) ? BLACK0  : WHITE0);
#endif
			}
#ifdef DISPLAY_180_DEGREE
			PreviousPicture = DataLinePrt;
#endif
			DataLineScan[(i>>2)] = ScanTable[(i%4)];
			// SPI (0x0a, line data....)
			epd_SPI_Send(0x0a, (uint8_t *)&COG_Line.uint8, COG_Parameters[EPD_Type_Index].DataLineSize);
			
			// SPI (0x02, 0x25)
			epd_SPI_Send_Byte(0x02, 0x2F);

			DataLineScan[(i>>2)] = 0;
		}
#if(!_GaugeFrameTimeMark)		
		if(COG_Parameters[EPD_Type_Index].FrameTime>0)
		{
			while(Currentframe>(epd_GetCurrentTimeTick() - StartClock));
		}
#endif
		//TestPin2Trigger();
		Currentframe=(uint32_t)(epd_GetCurrentTimeTick() - StartClock)+COG_Parameters[EPD_Type_Index].FrameTime ;
		cnt++;
	}while (StageTime>Currentframe);	
//	TestPin2Trigger();
	while(StageTime>(epd_GetCurrentTimeTick() - StartClock));
//	TestPin2Trigger();
//	TestPinTrigger();
}

//#pragma GCC optimize ("-O3")
static void Display_Stage_2 (uint8_t *PreviousPicture)
{
	uint16_t	i, j, k;				// general counters
	uint8_t		TempByte;			// Temporary storage for image data check
	uint16_t	StartClock;
#ifdef DISPLAY_180_DEGREE
	uint8_t     *DataLinePrt;
	DisplayOrgPtr = (PreviousPicture+(uint32_t)((COG_Parameters[EPD_Type_Index].VERTICAL-1)*COG_Parameters[EPD_Type_Index].HORIZONTAL));
#else
	DisplayOrgPtr = PreviousPicture;
#endif

//	TestPinTrigger();
	cnt = 0;
	Currentframe = (uint32_t)COG_Parameters[EPD_Type_Index].FrameTime;
	StartClock = epd_GetCurrentTimeTick();
	do
	{
//	    TestPin2Trigger();
	    PreviousPicture = DisplayOrgPtr;
#ifdef DISPLAY_180_DEGREE
		DataLinePrt = PreviousPicture;
#endif
		for (i = 0; i < COG_Parameters[EPD_Type_Index].VERTICAL; i++)			// for every line
		{
		    // SPI (0x04, ...)
			epd_SPI_Send_Byte(0x04, COG_Parameters[EPD_Type_Index].VoltageLevel);
		    k = COG_Parameters[EPD_Type_Index].HORIZONTAL-1;
			for (j = 0; j < COG_Parameters[EPD_Type_Index].HORIZONTAL; j++)		// make every bit in the line black
			{			
				TempByte =(*PreviousPicture++);							
#ifdef DISPLAY_180_DEGREE
				DataLineEven[j]	=      ((TempByte & 0x80) ? WHITE3  : NOTHING)
									 | ((TempByte & 0x20) ? WHITE2  : NOTHING)
									 | ((TempByte & 0x08) ? WHITE1  : NOTHING)
									 | ((TempByte & 0x02) ? WHITE0  : NOTHING);
				DataLineOdd[k--] =     ((TempByte & 0x01) ?  WHITE3 : NOTHING)
									 | ((TempByte & 0x04) ?  WHITE2 : NOTHING)
									 | ((TempByte & 0x10) ?  WHITE1 : NOTHING)
									 | ((TempByte & 0x40) ?  WHITE0 : NOTHING);
			    DataLinePrt--;
#else
			    DataLineOdd[j] =       ((TempByte & 0x80) ? WHITE3  : NOTHING)
									 | ((TempByte & 0x20) ? WHITE2  : NOTHING)
									 | ((TempByte & 0x08) ? WHITE1  : NOTHING)
									 | ((TempByte & 0x02) ? WHITE0  : NOTHING);				
				DataLineEven[k--] =    ((TempByte & 0x01) ?  WHITE3 : NOTHING)
									 | ((TempByte & 0x04) ?  WHITE2 : NOTHING)
									 | ((TempByte & 0x10) ?  WHITE1 : NOTHING)
									 | ((TempByte & 0x40) ?  WHITE0 : NOTHING);
#endif
			}
			
#ifdef DISPLAY_180_DEGREE
			PreviousPicture = DataLinePrt;
#endif
			DataLineScan[(i>>2)] = ScanTable[(i%4)];
			// SPI (0x0a, line data....)
			epd_SPI_Send(0x0a, (uint8_t *)&COG_Line.uint8, COG_Parameters[EPD_Type_Index].DataLineSize);
			
			epd_SPI_Send_Byte(0x02, 0x25);

			DataLineScan[(i>>2)] = 0;
		}
#if(!_GaugeFrameTimeMark)		
		if(COG_Parameters[EPD_Type_Index].FrameTime>0)
		{
			while(Currentframe>(epd_GetCurrentTimeTick() - StartClock));
		}
#endif		
		//TestPin2Trigger();
		Currentframe=(uint32_t)(epd_GetCurrentTimeTick() - StartClock)+COG_Parameters[EPD_Type_Index].FrameTime ;
		cnt++;
	}while (StageTime>Currentframe);
//	TestPin2Trigger();
	while(StageTime>(epd_GetCurrentTimeTick() - StartClock));
//	TestPin2Trigger();
//	TestPinTrigger();
}


//#pragma GCC optimize ("-O3")
static void Display_Stage_3 (uint8_t *Picture)
{
	uint16_t	i, j,k;				// general counters
	uint8_t		TempByte;			// Temporary storage for image data check
	uint16_t	StartClock;
#ifdef DISPLAY_180_DEGREE
	uint8_t     *DataLinePrt;
	DisplayOrgPtr = (Picture+(uint32_t)((COG_Parameters[EPD_Type_Index].VERTICAL-1)*COG_Parameters[EPD_Type_Index].HORIZONTAL));
#else
	DisplayOrgPtr = Picture;
#endif
	Currentframe = (uint32_t)COG_Parameters[EPD_Type_Index].FrameTime;
	cnt = 0;
//	TestPinTrigger();
	StartClock = epd_GetCurrentTimeTick();
	do
	{
//		TestPin2Trigger();
	    Picture = DisplayOrgPtr;
#ifdef DISPLAY_180_DEGREE
		DataLinePrt = Picture;
#endif
		for (i = 0; i < COG_Parameters[EPD_Type_Index].VERTICAL; i++)			// for every line
		{			
		    // SPI (0x04, 0x03)
			epd_SPI_Send_Byte(0x04, COG_Parameters[EPD_Type_Index].VoltageLevel);
			k = COG_Parameters[EPD_Type_Index].HORIZONTAL-1;
			for (j = 0; j < COG_Parameters[EPD_Type_Index].HORIZONTAL; j++)		// check every bit in the line
			{				
				TempByte = (*Picture++);
#ifdef DISPLAY_180_DEGREE
				DataLineEven[j] =     ((TempByte & 0x80) ? BLACK3  : NOTHING)
									| ((TempByte & 0x20) ? BLACK2  : NOTHING )
									| ((TempByte & 0x08) ? BLACK1  : NOTHING )
									| ((TempByte & 0x02) ? BLACK0  : NOTHING );
				
				DataLineOdd[k--] =    ((TempByte & 0x01) ? BLACK3  : NOTHING)
									| ((TempByte & 0x04) ? BLACK2  : NOTHING )
									| ((TempByte & 0x10) ? BLACK1  : NOTHING )
									| ((TempByte & 0x40) ? BLACK0  : NOTHING );				
				DataLinePrt--;
#else
				DataLineOdd[j] =      ((TempByte & 0x80) ? BLACK3  : NOTHING)
									| ((TempByte & 0x20) ? BLACK2  : NOTHING )
									| ((TempByte & 0x08) ? BLACK1  : NOTHING )
									| ((TempByte & 0x02) ? BLACK0  : NOTHING );

				DataLineEven[k--] =   ((TempByte & 0x01) ? BLACK3  : NOTHING)
									| ((TempByte & 0x04) ? BLACK2  : NOTHING )
									| ((TempByte & 0x10) ? BLACK1  : NOTHING )
									| ((TempByte & 0x40) ? BLACK0  : NOTHING );
#endif
			}					
#ifdef DISPLAY_180_DEGREE
			Picture = DataLinePrt;
#endif
			DataLineScan[(i>>2)] = ScanTable[(i%4)];
			// SPI (0x0a, line data....)
			epd_SPI_Send(0x0a, (uint8_t *)&COG_Line.uint8, COG_Parameters[EPD_Type_Index].DataLineSize);
			
			epd_SPI_Send_Byte(0x02, 0x2F);

			DataLineScan[(i>>2)] = 0;
		}
#if(!_GaugeFrameTimeMark)		
		if(COG_Parameters[EPD_Type_Index].FrameTime>0)
		{
			while(Currentframe>(epd_GetCurrentTimeTick() - StartClock));
		}
#endif		
		//TestPin2Trigger();
		Currentframe=(uint32_t)(epd_GetCurrentTimeTick() - StartClock)+COG_Parameters[EPD_Type_Index].FrameTime ;
		cnt++;
	}while (StageTime>Currentframe);
//	TestPin2Trigger();
	while(StageTime>(epd_GetCurrentTimeTick() - StartClock));
//	TestPin2Trigger();
//	TestPinTrigger();
}

//#pragma GCC optimize ("-O3")
static void Display_Stage_4 (uint8_t *Picture)
{
	uint16_t	i, j,k;				// general counters
	uint8_t		TempByte;			// Temporary storage for image data check
	uint16_t	StartClock;
#ifdef DISPLAY_180_DEGREE
	uint8_t		*DataLinePrt;
	DisplayOrgPtr = (Picture+(uint32_t)((COG_Parameters[EPD_Type_Index].VERTICAL-1)*COG_Parameters[EPD_Type_Index].HORIZONTAL));
#else
	DisplayOrgPtr = Picture;
#endif
	Currentframe = (uint32_t)COG_Parameters[EPD_Type_Index].FrameTime;
	cnt = 0;
//	TestPinTrigger();
	
	StartClock = epd_GetCurrentTimeTick();
	do
	{
//		TestPin2Trigger();
	    Picture = DisplayOrgPtr;
#ifdef DISPLAY_180_DEGREE
		DataLinePrt = Picture;
#endif
		for (i = 0; i < COG_Parameters[EPD_Type_Index].VERTICAL; i++)			// for every line
		{			
			// SPI (0x04, ...)
			epd_SPI_Send_Byte(0x04, COG_Parameters[EPD_Type_Index].VoltageLevel);
		    k = COG_Parameters[EPD_Type_Index].HORIZONTAL-1;
			for (j = 0; j < COG_Parameters[EPD_Type_Index].HORIZONTAL; j++)		// check every bit in the line
			{					
			    TempByte =(*Picture++);			  				
#ifdef DISPLAY_180_DEGREE
				 DataLineEven[j] =       ((TempByte & 0x80) ? WHITE3  : BLACK3 )
									   | ((TempByte & 0x20) ? WHITE2  : BLACK2 )
									   | ((TempByte & 0x08) ? WHITE1  : BLACK1 )
									   | ((TempByte & 0x02) ? WHITE0  : BLACK0 );
				
				 DataLineOdd[k--] =      ((TempByte & 0x01) ? WHITE3  : BLACK3 )
									   | ((TempByte & 0x04) ? WHITE2  : BLACK2 )
									   | ((TempByte & 0x10) ? WHITE1  : BLACK1 )
									   | ((TempByte & 0x40) ? WHITE0  : BLACK0 );			
				DataLinePrt--;
#else
				 DataLineOdd[j] =        ((TempByte & 0x80) ? WHITE3  : BLACK3 )
									   | ((TempByte & 0x20) ? WHITE2  : BLACK2 )
									   | ((TempByte & 0x08) ? WHITE1  : BLACK1 )
									   | ((TempByte & 0x02) ? WHITE0  : BLACK0 );

				 DataLineEven[k--] =     ((TempByte & 0x01) ? WHITE3  : BLACK3 )
									   | ((TempByte & 0x04) ? WHITE2  : BLACK2 )
									   | ((TempByte & 0x10) ? WHITE1  : BLACK1 )
									   | ((TempByte & 0x40) ? WHITE0  : BLACK0 );
#endif
			}		
#ifdef DISPLAY_180_DEGREE
			Picture = DataLinePrt;
#endif
			DataLineScan[(i>>2)] = ScanTable[(i%4)];
			// SPI (0x0a, line data....)
			epd_SPI_Send(0x0a, (uint8_t *)&COG_Line.uint8, COG_Parameters[EPD_Type_Index].DataLineSize);
			
			epd_SPI_Send_Byte(0x02, 0x2F);

		    DataLineScan[(i>>2)] = 0;
		}
#if(!_GaugeFrameTimeMark)		
		if(COG_Parameters[EPD_Type_Index].FrameTime>0)
		{
			while(Currentframe>(epd_GetCurrentTimeTick() - StartClock));
		}
#endif		
		//TestPin2Trigger();
		Currentframe=(uint32_t)(epd_GetCurrentTimeTick() - StartClock)+COG_Parameters[EPD_Type_Index].FrameTime ;
		cnt++;
	}while (StageTime>Currentframe);

//	TestPin2Trigger();
	while(StageTime>(epd_GetCurrentTimeTick() - StartClock));
//	TestPin2Trigger();
//	TestPinTrigger();
}

static void Display_Nothing (void)
{
	uint16_t	i;					// general counters

	for (i = 0; i <  COG_Parameters[EPD_Type_Index].HORIZONTAL; i++)		// make every bit in the line white
	{
		DataLineEven[i] = 0x00;
		DataLineOdd[i]  = 0x00;
	}

	for (i = 0; i < COG_Parameters[EPD_Type_Index].VERTICAL; i++)			// for every line
	{
		epd_SPI_Send_Byte(0x04, 0x03);
		DataLineScan[(i>>2)] = ScanTable[(i%4)];
		// SPI (0x0a, line data....)
		epd_SPI_Send(0x0a, (uint8_t *)&COG_Line.uint8, COG_Parameters[EPD_Type_Index].DataLineSize);

		epd_SPI_Send_Byte(0x02, 0x2F);
	}
}

static void Dummy_line(void)
{
	uint16_t	i;

	for (i = 0; i < COG_Parameters[EPD_Type_Index].DataLineSize; i++)
	{
		COG_Line.uint8[i] = 0x00;
	}

	epd_SPI_Send_Byte(0x04, 0x03);

	// SPI (0x0a, line data....)
	epd_SPI_Send(0x0a, (uint8_t *)&COG_Line.uint8, COG_Parameters[EPD_Type_Index].DataLineSize);

	epd_SPI_Send_Byte(0x02, 0x2F);
}


/******************************************************************************
 * Public functions
 *****************************************************************************/
void epd_HwInit(void)
{
	epd_InitDisplayHardware();
}

void epd_PowerOn(void)
{
	epd_discharge_low();
	epd_rst_low();
	epd_cs_low();
	epd_spi_init();

	epd_pwm_active(5);

	epd_panelon_on();

	epd_pwm_active(10);

	epd_cs_high();

	//epd_border_high();

	epd_rst_high();

	epd_pwm_active(5);

	epd_rst_low();

	epd_pwm_active(5);

	epd_rst_high();

	epd_pwm_active(5);
}

void epd_InitializeDriver(uint8_t EPDIndex)
{
	uint8_t SendBuffer[2];
	uint16_t k;

	EPD_Type_Index = EPDIndex;

	//Data Line Clear
	for (k = 0; k <= __LineDataSize; k ++)
	{
		COG_Line.uint8[k] = 0x00;
	}
	Driver_TypeSelect(EPDIndex);
	k = 0;

	SetTemperature_Factor(EPDIndex);

	/*while (DRIVERBUSY_Get())
	{
		delayT32B0Us(500);
		if((k++)>=1000) return;//wait 500 ms
	}
	*/

	// SPI (0x01, 0x0000, 0x0000, 0x01ff, 0xe000)
	epd_SPI_Send(0x01, (uint8_t *)&COG_Parameters[EPDIndex].ChannelSelect, 8);

	epd_SPI_Send_Byte(0x06, 0xff);
	epd_SPI_Send_Byte(0x07, 0x9d);
	epd_SPI_Send_Byte(0x08, 0x00);

	// SPI (0x09, 0xd000)
	SendBuffer[0] = 0xd0;
	SendBuffer[1] = 0x00;
	epd_SPI_Send(0x09, SendBuffer, 2);

	epd_SPI_Send_Byte(0x04,COG_Parameters[EPDIndex].VoltageLevel);

	epd_SPI_Send_Byte(0x03, 0x01);
	epd_SPI_Send_Byte(0x03, 0x00);

	epd_pwm_active(5);

	epd_SPI_Send_Byte(0x05, 0x01);

	epd_pwm_active(30);

	epd_SPI_Send_Byte(0x05, 0x03);
	epd_delay_ms(30);
	epd_SPI_Send_Byte(0x05, 0x0f);
	epd_delay_ms(30);
	epd_SPI_Send_Byte(0x02, 0x24);
}

void epd_Display (uint8_t *pNewImg, uint8_t *pPrevImg)
{	
	//COG Process - update display in four steps
	Display_Stage_1(pPrevImg);
	Display_Stage_2(pPrevImg);
	Display_Stage_3(pNewImg);
	Display_Stage_4(pNewImg);
}

void epd_PowerOff (void)
{
	Display_Nothing();
	Dummy_line();
	epd_delay_ms(25);

	//border_low();
	//epd_delay_ms(_30ms);
	//border_high();
	
	epd_SPI_Send_Byte(0x03, 0x01);
	epd_SPI_Send_Byte(0x02, 0x05);
	epd_SPI_Send_Byte(0x05, 0x0E);
	epd_SPI_Send_Byte(0x05, 0x02);
	epd_SPI_Send_Byte(0x04, 0x0C);
	epd_delay_ms(120);
	epd_SPI_Send_Byte(0x05, 0x00);
	epd_SPI_Send_Byte(0x07, 0x0D);
	epd_SPI_Send_Byte(0x04, 0x50);
	epd_delay_ms(40);
	epd_SPI_Send_Byte(0x04, 0xA0);
	epd_delay_ms(40);
	epd_SPI_Send_Byte(0x04, 0x00);
	
	epd_rst_low();
	epd_cs_low();
	epd_spi_detach();
	epd_panelon_off();
	
	//epd_border_low();
	
	epd_discharge_high();
	epd_delay_ms(150);
    epd_discharge_low();
}



