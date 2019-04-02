#include "stm32f10x.h"
#include "usart.h"	
#include "stm_flash.h"
#include "iar.h"

#define  test 0          //0������   1Ϊ����
//#define  normal        1  //0����
/**************************************************
                  ��������
model :  stm32f103c8t6       flash :   64K

bootloader :   5K          0x8000000-0x8001400     0-5K

APP1       :   25K         0x8001800-0x8008800    6k-34k

APP2       :   25K         0x8008c00-0X800FC00    35k-63K     

***************************************************/

/***************************************************
                  ������ַ����  (0X800FC00-0x8010000 64K)
uint8_t APP1.jump_flag                          0X800FC00            2
uint8_t APP1.upgrade_flag                       0X800FC06                      
uint8_t APP1.upgrade_version                    0X800FC10           2 
uint8_t APP1.Total_blocks                       0X800FC20             2  
uint8_t APP1.local_version                      0X800FC30             2
uint16_t APP1.NOW_blocks                        0X800FC40
uint16_t APP1.flash_blocks                      0X800FC60 

*****************************************************/

#define  APP1          0x8001800     //
#define  APP2          0x8008c00    //

int main ()
{
	
#if test
	  uint8_t i, a,b;
	  uint8_t n[5]={0x01,0x02,0x03,0x04,0x05};
		uint8_t m[5]={0};
	     a=255;
		uart_init(115200);	 
	
	  STMFLASH_Write ( 0x8009000, (uint16_t *)&a,1)	;
	 
	  STMFLASH_Write ( 0x8009002, (uint16_t *)n,3);
		 STMFLASH_Read  ( 0x8009000, (uint16_t *)&b,1) ;  	
		STMFLASH_Read  ( 0x8009002, (uint16_t *)m,3);
	  printf("b=%d\r\n",b);
	  printf("�ֹ�\r\n");
	  
     for(i=0;i<5;i++)
  {
     printf("m=%d\r\n",m[i]);

	}		
 #else
	
//	 volatile uint8_t   update_flag;            // 1 A������    2  B������
//   volatile	uint8_t   Jump_APP1_successful;   // 1ʧ��    2�ɹ�  APP1
//	 volatile	uint8_t   Jump_APP2_successful;   // 1ʧ��    2�ɹ�  APP2
//	 volatile uint16_t  Hop_count=0;           //��ת����
//		update_flag=1;
//		uart_init(115200);	 
//	 printf("�ֹ�\r\n");
////		update_flag=1;
////	 STMFLASH_Write  ( 0X800FC00, (uint16_t *)&update_flag,1);
//	 STMFLASH_Read  (  0X800FC00, (uint16_t *)&update_flag,1);
//	 printf("update_flag=%d\r\n",update_flag);
//	
  #endif
	while(1)
	{        
//			 if(update_flag==1)
//			 {
//		
//			   __disable_irq() ;
//					iap_load_app(APP1);
//				 			iap_load_app(APP2);
//					
//			 }
//			 else if(update_flag==2)
//			 {
//			
//__disable_irq() ;
//				iap_load_app(APP2);
//								iap_load_app(APP1);
//			 }
//			 else
//			 {
//	__disable_irq() ;
				 iap_load_app(APP1);
//				 
//			 }
		 
	}
	
	
	
	
	
}

