/******************** Milk tea machine **************************
 * author  ��gfb
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
 * ��������NVIC_Configuration
 * ����  ������NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
 * ����  ����
 * ���  ����
 */
void NVIC_Configuration(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}
