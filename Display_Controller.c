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
//#include "type.h"
//#include "gpio.h"
#include "Display_Controller.h"

/******************************************************************************
 * Global Functions
 *****************************************************************************/

void epd_DisplayImg(EPDType_t EPDType, uint8_t *pNewImg, uint8_t *pPrevImg)
{			
	//always initialize display (has been powered off before)
	epd_HwInit();
	epd_PowerOn();
	epd_InitializeDriver(EPDType);

	//display new picture, but first remove old one
	epd_Display(pNewImg, pPrevImg);

	//power down display - picture still displayed
	epd_PowerOff();
}

