#ifndef     __BSP_USART_H_
#define     __BSP_USART_H_
#include "stm32f1xx_hal.h"


#define  RX_LEN  1000

	typedef struct  
{  
uint8_t  RX_flag;        //IDLE receive flag
uint16_t RX_Size;          //receive length
uint8_t  RX_pData[RX_LEN];
}USART_RECEIVETYPE; 









#endif

