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

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "bsp.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <wiringPiSPI.h>

#include <string.h>

/******************************************************************************
 * Typedefs and defines
 *****************************************************************************/

#define LM75A_I2C_ADDR 0x49 //(0x49 << 1)
#define LM75A_CMD_TEMP 0x00
#define DISPLAY_BUF_SZ (256)

/******************************************************************************
 * Local variables
 *****************************************************************************/

// I2C device handle
static int gI2cFd = -1;

// mapping between Serial Expansion connector and Raspberry Pi
// GPIOs. Raspberry Pi GPIO numbering according to wiringPi
static int pinMap[BSP_PIN_SZ*2] = 
{
  // [SEC]   | [PI GPIO#]
  BSP_PIN_6,   6,  // CS
  BSP_PIN_7,   0,  // Busy
  BSP_PIN_8,  -1,  // Not Used
  BSP_PIN_11,  1,  // PWM
  BSP_PIN_12,  5,  // RST
  BSP_PIN_13,  3,  // PWR_Ctrl
  BSP_PIN_14,  4,  // DISCHARGE
};

// buffer used when writing to display via SPI
static uint8_t displayBuf[DISPLAY_BUF_SZ];

/******************************************************************************
 * Local functions
 *****************************************************************************/

static int32_t lm75a_readTemp(void)
{
  uint8_t temp[2];
  int16_t t = 0;

  wiringPiI2CWrite(gI2cFd, LM75A_CMD_TEMP);
  temp[0] = wiringPiI2CRead(gI2cFd);
  temp[1] = wiringPiI2CRead(gI2cFd);
  

  // 11 MSB bits used. Celcius is calculated as Temp data * 1/8 
  t = ((temp[0] << 8) | (temp[1]));

  return ((t * 100) >> 8);
}


/******************************************************************************
 * Public functions
 *****************************************************************************/
 
 
/******************************************************************************
 *
 * Description:
 *   BSP initialize function. Must be called before any other BSP related
 *   functions.
 *
 *****************************************************************************/
int bsp_init(void)
{

  if (wiringPiSetup() < 0) {
    printf("bsp_init: failed to initialize wiringPi\n");
    return -1;
  }

  if ((gI2cFd = wiringPiI2CSetup(LM75A_I2C_ADDR)) < 0) {
    printf("bsp_init: failed to initialize I2C\n");
    return -1;
  }
  
  if (wiringPiSPISetup(0, 1000000) < 0) {
    printf("bsp_init: failed to initialize SPI\n");
    return -1;
  }
  
  return 0;
}

/******************************************************************************
 *
 * Description:
 *   Get number of milliseconds
 *
 *****************************************************************************/
uint32_t bsp_getMsTicks(void)
{
  return millis();
}

/******************************************************************************
 *
 * Description:
 *   Initialize SPI
 *
 *****************************************************************************/
void bsp_spiInit(void)
{
  // initialized in bsp_init
}

/******************************************************************************
 *
 * Description:
 *   Write data to the display
 *
 *****************************************************************************/
void bsp_writeToDisplay(uint8_t* data, uint16_t len)
{
  if (len > DISPLAY_BUF_SZ) {
    printf("bsp_writeToDisplay: ERROR len=%d > %d\n", len, DISPLAY_BUF_SZ);
    return;
  }
  
  memcpy(displayBuf, data, len);
  wiringPiSPIDataRW(0, displayBuf, len);
    
}

/******************************************************************************
 *
 * Description:
 *   Delay specified number of milliseconds
 *
 * Params:
 *   [in] ms - number of milliseconds to delay
 *
 *****************************************************************************/
void bsp_delayMs(uint32_t ms)
{
  delay(ms);
}

/******************************************************************************
 *
 * Description:
 *   Delay specified number of microseconds
 *
 * Params:
 *   [in] us - number of microseconds to delay
 *
 *****************************************************************************/
void bsp_delayUs(uint32_t us)
{
  delayMicroseconds(us);
}

/******************************************************************************
 *
 * Description:
 *   Read temperature
 *
 * Params:
 *   [out] temperature - (int16_t) temperature in Celsius
 *
 *****************************************************************************/
int16_t bsp_getTemp(void)
{
  lm75a_readTemp();
  bsp_delayMs(5);
  return (int16_t)(lm75a_readTemp()/100);
}

/******************************************************************************
 *
 * Description:
 *   Set the PIN mode of a specific pin
 *
 *****************************************************************************/
void bsp_pinMode(bsp_pin_t pin, bps_pinMode_t mode)
{
  int m = INPUT;
  int p = 0;
  
  if (pin >= BSP_PIN_SZ) return;
  
  if (mode == BSP_PINMODE_OUTPUT) {
    m = OUTPUT;
  }
  
  p = pinMap[pin*2+1];
  
  pinMode(p, m); 
}

/******************************************************************************
 *
 * Description:
 *   Set PIN to high (1) of low (0) value
 *
 *****************************************************************************/
void bsp_pinOut(bsp_pin_t pin, int value)
{
  int p = 0;
  int v = LOW;
  
  if (pin >= BSP_PIN_SZ) return;  
  
  if (value != 0) {
    v = HIGH;
  }
  
  p = pinMap[pin*2+1];  
  
  digitalWrite(p, v);
}


