
#include  "app.h"
#include "bsp_led.h"



void  system_run()
{
	
	 if(Read_det_Value())
	 {
		    AC_LED_OPEN();
		    DC_LED_CLOSE();
		   app_runing(4, 1, 255,1,10);
	 }
	else
	{
		
		  AC_LED_CLOSE();
		   DC_LED_OPEN();
		  app_runing(4, 2, 255,0,10);
		
	}
	
	
	
}