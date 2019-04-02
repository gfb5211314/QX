
//STM32升级程序
#include "iar.h"
#include "sys.h"


// 跳转APP程序
void iap_load_app(u32 appxaddr)
{   
	
	    appdress   jump2app;
if(((*(vu32*)appxaddr)&0x2FFE0000)==0x20000000) //检查栈顶地址是否合法， 0x20000000是SRAM是起始地址,也就是程序栈顶位
{ 
  jump2app=(appdress)*(vu32*)(appxaddr+4);//把地址强转为函数指针  //复位地址

	
	__set_MSP(*(__IO uint32_t*) appxaddr);//设置APP堆栈指针，用户代码区第一个字为堆栈顶位置
	
  jump2app();  //跳转APP程序 复位地址
}
}




//发送字符串
	void uranium(char *s)
	{		
while(*s!='\0')
{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET); 
		USART_SendData(USART2 ,*s++);//发送当前字符
}
}