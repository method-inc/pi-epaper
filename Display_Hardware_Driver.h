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

#ifndef __DISPLAY_HARDWARE_DRIVER_H_
#define __DISPLAY_HARDWARE_DRIVER_H_

/******************************************************************************
 * Includes
 *****************************************************************************/

#include "bsp.h"


/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/

//Pin 12 on Serial Expansion Connector
// #define RST_DIR_OUT			SEC14_PIN12_SET_OUTP
// #define RST_SET_HIGH		SEC14_PIN12_SET_HIGH
// #define RST_SET_LOW			SEC14_PIN12_SET_LOW

//Pin 14 on Serial Expansion Connector
// #define DISCHARGE_DIR_OUT	SEC14_PIN14_SET_OUTP
// #define DISCHARGE_SET_HIGH	SEC14_PIN14_SET_HIGH
// #define DISCHARGE_SET_LOW	SEC14_PIN14_SET_LOW

//Pin 6 on Serial Expansion Connector
// #define CS_DIR_OUT			SEC14_PIN6_SET_OUTP
// #define CS_SET_HIGH			SEC14_PIN6_SET_HIGH
// #define CS_SET_LOW			SEC14_PIN6_SET_LOW

//Pin 13 on Serial Expansion Connector
// #define PANELON_DIR_OUT		SEC14_PIN13_SET_OUTP
// #define PANELON_SET_HIGH	SEC14_PIN13_SET_HIGH
// #define PANELON_SET_LOW		SEC14_PIN13_SET_LOW

//Pin 7 on Serial Expansion Connector
// #define DRIVERBUSY_DIR_IN	SEC14_PIN7_SET_INP
// #define DRIVERBUSY_GET		SEC14_PIN7_INP

//Not implemented
// #define BORDER_DIR_OUT
// #define BORDER_SET_HIGH
// #define BORDER_SET_LOW

//Pin 11 on Serial Expansion Connector
// #define PWM_DIR_OUT			SEC14_PIN11_SET_OUTP
// #define PWM_SET_HIGH		SEC14_PIN11_SET_HIGH
// #define PWM_SET_LOW			SEC14_PIN11_SET_LOW


/******************************************************************************
 * Prototypes
 *****************************************************************************/
void epd_cs_high(void);
void epd_cs_low(void);
void epd_rst_high(void);
void epd_rst_low(void);
void epd_discharge_high(void);
void epd_discharge_low(void);
void epd_panelon_off(void);
void epd_panelon_on(void);

void epd_pwm_active(uint16_t delayInMs);

void epd_border_high(void);
void epd_border_low(void);

void epd_TestPinLow(void);
void epd_TestPinHigh(void);
void epd_TestPinTrigger(void);

void epd_TestPin2Low(void);
void epd_TestPin2High(void);
void epd_TestPin2Trigger(void);

void epd_delay_ms(uint32_t Time);
int16_t epd_get_temperature(void);
uint32_t epd_GetCurrentTimeTick(void);

/***************************************************************************/
void epd_spi_detach(void);
void epd_spi_init (void);
void epd_SPI_Send (unsigned char Register, unsigned char *Data, unsigned Length);
void epd_SPI_Send_Byte (uint8_t Register, uint8_t Data);
void epd_InitDisplayHardware (void);

/****************************************************************************/

#endif	//__DISPLAY_HARDWARE_DRIVER_H_
