/*****************************************************************************
 *
 *   Copyright(C) 2012, Embedded Artists AB
 *   All rights reserved.
 *
 ******************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * Embedded Artists AB assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. Embedded Artists AB
 * reserves the right to make changes in the software without
 * notification. Embedded Artists AB also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 *****************************************************************************/
#ifndef __BSP_H__
#define __BSP_H__

/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

// pin numbering is for the Serial Expansion Connector
typedef enum
{
  BSP_PIN_6 = 0,
  BSP_PIN_7,
  BSP_PIN_8,
  BSP_PIN_11,
  BSP_PIN_12,
  BSP_PIN_13,
  BSP_PIN_14,
  BSP_PIN_SZ // must be last 
} bsp_pin_t;

typedef enum
{
  BSP_PINMODE_INPUT = 0,
  BSP_PINMODE_OUTPUT
} bps_pinMode_t;

/******************************************************************************
 * Prototypes
 *****************************************************************************/
int bsp_init(void);
void bsp_spiInit(void);
void bsp_writeToDisplay(uint8_t* data, uint16_t len);
uint32_t bsp_getMsTicks(void);
void bsp_delayMs(uint32_t ms);
void bsp_delayUs(uint32_t us);
int16_t bsp_getTemp(void);
void bsp_pinMode(bsp_pin_t pin, bps_pinMode_t mode);
void bsp_pinOut(bsp_pin_t pin, int value);


#endif	//__BSP_H__
