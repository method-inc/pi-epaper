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

#ifndef __Display_Controller_H_
#define __Display_Controller_H_

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "bsp.h"
#include "Display_COG_Process.h"

/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/
#define DISPLAY_IN_USE 	EPD_TYPE_270

/******************************************************************************
 * Prototypes
 *****************************************************************************/
void epd_DisplayImg(EPDType_t EPDType, uint8_t *pNewImg, uint8_t *pPrevImg);

#endif //__Display_Controller_H_
