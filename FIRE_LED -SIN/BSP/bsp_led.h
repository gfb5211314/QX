#ifndef __BSP_LED_H_
#define __BSP_LED_H_

#include "stm32f1xx_hal.h"

extern uint8_t   Read_det_Value(void);


extern void AC_LED_OPEN(void);


extern void AC_LED_CLOSE(void);

extern void DC_LED_OPEN(void);



extern void DC_LED_CLOSE(void);


#endif

