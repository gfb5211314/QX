
//STM32��������
#include "iar.h"
#include "sys.h"


// ��תAPP����
void iap_load_app(u32 appxaddr)
{   
	
	    appdress   jump2app;
if(((*(vu32*)appxaddr)&0x2FFE0000)==0x20000000) //���ջ����ַ�Ƿ�Ϸ��� 0x20000000��SRAM����ʼ��ַ,Ҳ���ǳ���ջ��λ
{ 
  jump2app=(appdress)*(vu32*)(appxaddr+4);//�ѵ�ַǿתΪ����ָ��  //��λ��ַ

	
	__set_MSP(*(__IO uint32_t*) appxaddr);//����APP��ջָ�룬�û���������һ����Ϊ��ջ��λ��
	
  jump2app();  //��תAPP���� ��λ��ַ
}
}




//�����ַ���
	void uranium(char *s)
	{		
while(*s!='\0')
{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET); 
		USART_SendData(USART2 ,*s++);//���͵�ǰ�ַ�
}
}