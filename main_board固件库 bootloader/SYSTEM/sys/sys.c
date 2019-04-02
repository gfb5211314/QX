/******************** Milk tea machine **************************
 * author  ：gfb
 * dates   : 2017/12/22
 * Version : V1.00
 * file    : main.c
 *describe : The system business logic handles the main function
*******************************************************************************/
/*-----------------------------------------------------------------------------
   author           Version           data       content 
	 gfb                V1.00         2017/12/22    MAIN.C
	 
------------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "sys.h"

/*
 * 函数名：NVIC_Configuration
 * 描述  ：设置NVIC中断分组2:2位抢占优先级，2位响应优先级
 * 输入  ：无
 * 输出  ：无
 */
void NVIC_Configuration(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}
