
//STM32��������
#ifndef    __IAR_H_
#define   __IAR_H_


#include "sys.h"

//����һ��ָ������ָ�������� appdress
typedef  void (*appdress )(void);  //����  ����ָ�������� APPDRESS   P2  ��ôP2����ָ������ָ�룬������ָ��CHAR ��INT���͵�ָ��

void iap_load_app(u32 appxaddr);
	 				    
#endif