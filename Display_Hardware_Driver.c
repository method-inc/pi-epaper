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
#include "Display_Hardware_Driver.h"


/******************************************************************************
 * Public functions
 *****************************************************************************/
void epd_cs_high(void)
{
  // CS_SET_HIGH;
  bsp_pinOut(BSP_PIN_6, 1);  
}

void epd_cs_low(void)
{
  // CS_SET_LOW;
  bsp_pinOut(BSP_PIN_6, 0);  
}

void epd_rst_high(void)
{
  // RST_SET_HIGH;
  bsp_pinOut(BSP_PIN_12, 1);
}

void epd_rst_low(void)
{
  // RST_SET_LOW;
  bsp_pinOut(BSP_PIN_12, 0);  
}

void epd_discharge_high(void)
{
  // DISCHARGE_SET_HIGH;
  bsp_pinOut(BSP_PIN_14, 1);
}

void epd_discharge_low(void)
{
  // DISCHARGE_SET_LOW;
  bsp_pinOut(BSP_PIN_14, 0);
}

void epd_panelon_off(void)
{
  // PANELON_SET_LOW;
  bsp_pinOut(BSP_PIN_13, 0);
}

void epd_panelon_on(void)
{
  // PANELON_SET_HIGH;
  bsp_pinOut(BSP_PIN_13, 1);
}

void epd_border_high(void)
{
  // BORDER_SET_HIGH;
}

void epd_border_low(void)
{
  // BORDER_SET_LOW;
}

void epd_delay_ms(uint32_t Time)
{
	bsp_delayMs(Time);
}

int16_t epd_get_temperature(void)
{
	return bsp_getTemp();
}

uint32_t epd_GetCurrentTimeTick(void)
{
	return bsp_getMsTicks();
}

//******************************************************************
//PWM  Configuration/Control
//******************************************************************
#if 0
volatile uint32_t period = 500;  //96Khz PWM frequency

void epd_pwm_disable(void)
{
	disable_timer16(1);
	LPC_IOCON->PIO1_9  &= 0xFC;//Disable PWM,set general IO 
	PWM_DIR_OUT();
	PWM_SET_LOW();
}

void epd_pwm_active(void)
{
	PWM_SET_HIGH();
	/* Initialize the PWM in timer32_1 enabling match0 output */
	init_timer16PWM(1, period, MATCH0,0);
	setMatch_timer16PWM (1, 1, period/8);
	enable_timer16(1);
}
#endif
void epd_pwm_active(uint16_t delayInMs)
{
  uint16_t numOfIterations;

  numOfIterations = delayInMs * 100;
  // PWM_DIR_OUT;
  bsp_pinMode(BSP_PIN_11, BSP_PINMODE_OUTPUT);
  for(; numOfIterations > 0; numOfIterations--)
  {
    // PWM_SET_HIGH;
    bsp_pinOut(BSP_PIN_11, 1);
    bsp_delayUs(5);	//100kHz (96kHz ideal)
    // PWM_SET_LOW;
    bsp_pinOut(BSP_PIN_11, 0);
    bsp_delayUs(5);
  }
}


//******************************************************************
//SPI  Configuration
//******************************************************************
void epd_spi_detach(void)
{
#if 0
	LPC_IOCON->PIO0_8			&= 0xFC;; /*  SSP I/O config */
	SPIMISO_DIR_OUT();
	SPIMISO_Set(Low);
	
	LPC_IOCON->PIO0_9			&= 0xFC; /* SSP MOSI */	
	SPIMOSI_DIR_OUT();
	SPIMOSI_Set(Low);
	
	LPC_IOCON->PIO2_11 			&= 0xFC; 
	SPICLK_DIR_OUT();			
	SPICLK_Set(Low);
#endif
}
void epd_spi_init (void)
{
  bsp_spiInit();
//SSP_IOConfig( SSP_PORT );	/* initialize SSP port */
//SSP_Init( SSP_PORT );
}

void epd_SPI_Send (unsigned char Register, unsigned char *Data, unsigned Length)
{
  uint8_t buf[2];

  epd_cs_low();
  buf[0] = 0x70;
  buf[1] = Register;
  bsp_writeToDisplay(buf, 2);
  epd_cs_high();
  bsp_delayUs(10);

  epd_cs_low();
  buf[0] = 0x72;
  bsp_writeToDisplay(buf, 1);
  bsp_writeToDisplay(Data, Length);
  epd_cs_high();
  bsp_delayUs(10);
}



void epd_SPI_Send_Byte (uint8_t Register, uint8_t Data)
{
  uint8_t buf[2];

  epd_cs_low();
  buf[0] = 0x70;
  buf[1] = Register;
  bsp_writeToDisplay(buf, 2);
  epd_cs_high();
  bsp_delayUs(10);

  epd_cs_low();
  buf[0] = 0x72;
  buf[1] = Data;
  bsp_writeToDisplay(buf, 2);
  epd_cs_high();
  bsp_delayUs(10);
}

void epd_InitDisplayHardware(void)
{
  // RST_DIR_OUT;
  bsp_pinMode(BSP_PIN_12, BSP_PINMODE_OUTPUT);
  // DISCHARGE_DIR_OUT;
  bsp_pinMode(BSP_PIN_14, BSP_PINMODE_OUTPUT);
  // CS_DIR_OUT;
  bsp_pinMode(BSP_PIN_6, BSP_PINMODE_OUTPUT);
  // PANELON_DIR_OUT;
  bsp_pinMode(BSP_PIN_13, BSP_PINMODE_OUTPUT);
  // DRIVERBUSY_DIR_IN;
  bsp_pinMode(BSP_PIN_7, BSP_PINMODE_INPUT);
  // BORDER_DIR_OUT;


  epd_panelon_off();
  epd_spi_init();
  epd_cs_low();
  //	epd_pwm_low();
  epd_pwm_active(0);	//set output low
  epd_rst_low();
  epd_discharge_low();

//	TESTPin_DIR_OUT();
//	TESTPin2_DIR_OUT();
}

//************************************************************************
void TestPinLow(void)
{
//	TESTPin_Set(0);
}
void TestPinHigh(void)
{
//	TESTPin_Set(1);
}
void TestPinTrigger(void)
{
//	if(TESTPin_Get())TestPinLow();
//	else TestPinHigh();
}
void TestPin2Low(void)
{
//	TESTPin2_Set(0);
}
void TestPin2High(void)
{
//	TESTPin2_Set(1);
}
void TestPin2Trigger(void)
{
//	if(TESTPin2_Get())TestPin2Low();
//	else TestPin2High();
}
