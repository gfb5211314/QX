
#include  "app.h"
#include "bsp_led.h"
#include   "bsp_ws2812.h"


extern uint8_t grad_cnt;
extern uint16_t color_init;

void  system_run()
{

    if(Read_det_Value())
    {
        AC_LED_OPEN();
        DC_LED_CLOSE();
        app_runing(180, 1, 255, 1, 10);

    }
    else
    {

        AC_LED_CLOSE();
        DC_LED_OPEN();
        app_runing(180, 2, 255, 0, 10);

    }



}