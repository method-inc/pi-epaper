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

#ifndef __Display_COG_Process_H_
#define __Display_COG_Process_H_

/******************************************************************************
 * Includes
 *****************************************************************************/

 #include "bsp.h"

/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/
#define BLACK0	(0x03)
#define BLACK1	(0x0C)
#define BLACK2	(0x30)
#define BLACK3	(0xc0)
#define WHITE0	(0x02)
#define WHITE1	(0x08)
#define WHITE2	(0x20)
#define WHITE3  (0x80)

#define NOTHING	(0x00)
#define SCANON	(0xC0)
#define __LineDataSize	111

/*
enum 
{
	_5ms,
	_10ms,
	_25ms,
	_30ms,
	_40ms,
	_120ms,
	_150ms,
	_300ms
};
*/

typedef enum
{
  EPDType_144 = 0,	//1.44" display
  EPDType_200 = 1,	//2.0"  display
  EPDType_270 = 2	//2.7"  display
} EPDType_t;
#define EPD_TYPE_144 	0
#define EPD_TYPE_200 	1
#define EPD_TYPE_270 	2

typedef struct
{
	uint8_t	Even[16];
	uint8_t	Scan[24];
	uint8_t	Odd [16];
} COG_144_LineData_t;

typedef struct
{
	uint8_t	Even[25];
	uint8_t	Scan[24];
	uint8_t	Odd [25];
	uint8_t	DummyData;
} COG_20_LineData_t;

typedef struct
{
	uint8_t	Even[33];
	uint8_t	Scan[44];
	uint8_t	Odd [33];
	uint8_t	DummyData;
} COG_27_LineData_t;

typedef union
{
	union
	{
		COG_144_LineData_t	COG_144LineData;
		COG_20_LineData_t	COG_20LineData;
		COG_27_LineData_t	COG_27LineData;
	} LineDatas;
    uint8_t					uint8[__LineDataSize];
} COG_LineDataPacket_t;

typedef struct
{
	uint8_t		ChannelSelect[8];
	uint8_t		VoltageLevel;
	uint16_t	HORIZONTAL;
	uint16_t	VERTICAL;
	uint8_t		DataLineSize;
	uint16_t	FrameTime;
	uint16_t	StageTime;
} COG_Parameters_t;

extern const COG_Parameters_t COG_Parameters[3];

/******************************************************************************
 * Prototypes
 *****************************************************************************/
void epd_HwInit(void);
void epd_PowerOn(void);
void epd_InitializeDriver(uint8_t EPDIndex);
void epd_Display(uint8_t *pNewImg, uint8_t *pPrevImg);
void epd_PowerOff(void);

#endif	//__Display_COG_Process_H_
