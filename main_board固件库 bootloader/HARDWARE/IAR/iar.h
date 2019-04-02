
//STM32升级程序
#ifndef    __IAR_H_
#define   __IAR_H_


#include "sys.h"

//定义一个指向函数的指针类型命 appdress
typedef  void (*appdress )(void);  //作用  函数指针类型名 APPDRESS   P2  那么P2就是指向函数的指针，而不是指向CHAR 和INT类型的指针

void iap_load_app(u32 appxaddr);
	 				    
#endif