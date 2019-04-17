/**
  ******************************************************************************
  * @file    ws2812_app.c
  * @author  FuBiao.Guo
  * @version V1.1.0
  * @date    2-4-2019
  * @brief   The modulation effect of ws2812 function file.
	**/

#include  "ws2812_app.h"
#include  "bsp_ws2812.h"
#include <stdlib.h>
#include "string.h"
#include   "bsp_ws2812.h"
#include "stm32f1xx_hal.h"
#include  "math.h"
#include   "bsp_dma.h"
#include "string.h"
#include <stdlib.h>
extern uint8_t   ws28128_color_buf[1000][3];

/**
  * @brief Randomly different colors
  * @param  uint16_t amount :  maximum LED number of ws28128
  * @retval None
  */


void ws2812_rand_light(volatile uint16_t  amount)
{
    uint16_t rand_data = 0, rand_color = 0;
    for(uint16_t i = 0; i < 360; i++)
    {
        rand_data = rand() % 256;
        rand_buff_data(180, i);
        DMA_WS2812_Reset();
        HAL_Delay(1);
        DMA_WS2812_light(180);
        HAL_Delay(30);
    }
}

/**
 * @brief  Each color to run
 * @param  uint16_t amount :  maximum LED number of ws28128
 * @param  uint16_t color_type_n :  Color number
 * @retval None
 */
/***********每个色循环往下跑**********未完成****/
void  arrange_display_one_run(volatile uint16_t  amount, uint16_t color_type_n)
{
    uint8_t r, g, b;

    for(uint16_t m = 1; m < 90; m++)
    {
        for(uint16_t i = 0; i < amount * 2; i++)
        {
            if(i < (amount / 4)) //i=44
            {

                HLS_TO_RGB_ONE(&r, &g, &b, m * 5, 0.30, 1, i, ws28128_color_buf); //5

            }

            else if(i > (amount / 4 - 1) && (i < (amount / 2))) //i=45  i=89
            {

                HLS_TO_RGB_ONE(&r, &g, &b, m * 5, 0.30, 1, i, ws28128_color_buf);  //2
                HLS_TO_RGB_ONE(&r, &g, &b, m * 10, 0.30, 1, i - (amount / 4), ws28128_color_buf); //1

            }
            else if(i > (amount / 2 - 1) && (i < (amount / 4) * 3))//i=90  i=135
            {

                HLS_TO_RGB_ONE(&r, &g, &b, m * 5, 0.30, 1, i, ws28128_color_buf); //3
                HLS_TO_RGB_ONE(&r, &g, &b, m * 10, 0.30, 1, i - (amount / 4), ws28128_color_buf); //2
                HLS_TO_RGB_ONE(&r, &g, &b, m * 20, 0.30, 1, (i - (amount / 4) * 2), ws28128_color_buf); //3
            }
            else if((i > (amount / 4) * 3 - 1) && (i < amount * 2)) //i=135  i=180
            {
                HLS_TO_RGB_ONE(&r, &g, &b, m * 5, 0.30, 1, i, ws28128_color_buf); //4ge
                HLS_TO_RGB_ONE(&r, &g, &b, m * 10, 0.30, 1, i - (amount / 4), ws28128_color_buf); //3
                HLS_TO_RGB_ONE(&r, &g, &b, m * 20, 0.30, 1, (i - (amount / 4) * 2), ws28128_color_buf); //2
                HLS_TO_RGB_ONE(&r, &g, &b, m * 30, 0.30, 1, (i - (amount / 4) * 3), ws28128_color_buf); //1
            }

            DMA_WS2812_Reset();
            HAL_Delay(1);
            DMA_WS2812_light(amount);
            HAL_Delay(100);

        }
    }
}
/**
 * @brief  Each color to run
 * @param  uint16_t amount :  maximum LED number of ws28128
 * @param  uint16_t color_type_n :  Color number
 * @retval None
 */
/***********每个色转一遍**************/
void  arrange_display_two_run(volatile uint16_t  amount)
{
    uint8_t r, g, b;
    for(uint16_t m = 0; m < 48; m++)
    {
        memset(ws28128_color_buf, 0, sizeof(ws28128_color_buf));
        for(uint16_t i = 0; i < amount / 2; i++)
        {

            HLS_TO_RGB_ONE(&r, &g, &b, m * 5, 0.30, 1, i, ws28128_color_buf);
            HLS_TO_RGB_ONE(&r, &g, &b, m * 5, 0.30, 1, amount - i, ws28128_color_buf);
            DMA_WS2812_Reset();
            HAL_Delay(1);
            DMA_WS2812_light(amount);
            HAL_Delay(10);
        }
        memset(ws28128_color_buf, 0, sizeof(ws28128_color_buf));
        for(uint16_t i = 0; i < amount / 2; i++)
        {

            HLS_TO_RGB_ONE(&r, &g, &b, m * 5, 0.30, 1, amount / 2 - i, ws28128_color_buf);
            HLS_TO_RGB_ONE(&r, &g, &b, m * 5, 0.30, 1, amount / 2 + i, ws28128_color_buf);
            DMA_WS2812_Reset();
            HAL_Delay(1);
            DMA_WS2812_light(amount);
            HAL_Delay(10);
        }

    }
}
/***多种颜色一起渐变***************/
void DMA_WS2812_Rampping(volatile uint16_t  amount, uint8_t pwm, colors_kind color)
{
    for(uint16_t m = 0; m < pwm; m++)
    {


        DMA_WS2812_SIN_More(amount, m, 1);
        DMA_WS2812_Reset();
        HAL_Delay(1);
        DMA_WS2812_light(amount);
        HAL_Delay(20);


    }

}
/***********色表**************/
void  arrange_display(volatile uint16_t  amount)
{
    uint8_t r, g, b;
    for(uint16_t m = 0; m < amount; m++)
    {

        HLS_TO_RGB_ONE(&r, &g, &b, m * 1.4, 0.30, 1, m, ws28128_color_buf);
        DMA_WS2812_Reset();
        HAL_Delay(1);
        DMA_WS2812_light(amount);
        HAL_Delay(30);
    }
    for(uint16_t m = 0; m < amount; m++)
    {

        HLS_TO_RGB_ONE(&r, &g, &b, m * 1.4, 0.30, 1, amount - m, ws28128_color_buf);
        DMA_WS2812_Reset();
        HAL_Delay(1);
        DMA_WS2812_light(amount);
        HAL_Delay(30);
    }


}
/***********色表2合一**************/
void  arrange_display_two(volatile uint16_t  amount)
{
    uint8_t r, g, b;
    for(uint16_t m = 0; m < amount; m++)
    {

        HLS_TO_RGB_ONE(&r, &g, &b, m * 1.4, 0.30, 1, m, ws28128_color_buf);
        HLS_TO_RGB_ONE(&r, &g, &b, m * 1.4, 0.30, 1, amount - m, ws28128_color_buf);
        DMA_WS2812_Reset();
        HAL_Delay(1);
        DMA_WS2812_light(amount);
        HAL_Delay(30);
    }
    for(uint16_t m = 0; m < amount; m++)
    {

        HLS_TO_RGB_ONE(&r, &g, &b, m * 1.4, 0.30, 1, amount - m, ws28128_color_buf);
        HLS_TO_RGB_ONE(&r, &g, &b, m * 1.4, 0.30, 1, m, ws28128_color_buf);
        DMA_WS2812_Reset();
        HAL_Delay(1);
        DMA_WS2812_light(amount);
        HAL_Delay(30);
    }


}
/*********单中颜色渐变***************/
void DMA_WS2812_Rampping_1(volatile uint16_t  amount, uint8_t pwm, colors_kind color)
{
    uint8_t r, g, b;
    for(uint16_t m = 0; m < pwm; m++)
    {

//      HLS2RGB( uint8_t *r, uint8_t *g, uint8_t *b, double h, double l, double s);
//        DMA_WS2812_SIN(amount, m,  color);
        //		HLS_TO_RGB( uint8_t *r, uint8_t *g, uint8_t *b, double h, double l, double s,uint16_t led_n,uint8_t arr[][3])
        HLS_TO_RGB_ALL(&r, &g, &b, m * 1.4, 0.30, 1, amount, ws28128_color_buf);
//		   	DMA_WS2812_SIN(amount, m,  2);
        DMA_WS2812_Reset();
        HAL_Delay(1);
        DMA_WS2812_light(amount);
        HAL_Delay(100);


    }


}

void DMA_WS2812_Running(volatile uint16_t  amount)
{
    for(uint16_t m = 0; m < amount; m++)
    {

        DMA_WS2812_data_shift(m);
        DMA_WS2812_Reset();
        HAL_Delay(1);
        DMA_WS2812_light(amount);
        HAL_Delay(50);
    }
//		  for(uint16_t m = 0; m < amount; m++)
//    {

//        DMA_WS2812_data_shift(amount-m);
//        DMA_WS2812_Reset();
//        HAL_Delay(1);
//        DMA_WS2812_light(amount);
//        HAL_Delay(50);
//    }



}
void DMA_WS2812_Run(volatile uint16_t  amount)
{
    for(uint16_t m = 0; m < amount; m++)
    {

        DMA_WS2812_data_shift_light(m, amount);
        DMA_WS2812_Reset();
        HAL_Delay(1);
        DMA_WS2812_light(amount);
        HAL_Delay(20);
    }
    memset(ws28128_color_buf, 0, sizeof(ws28128_color_buf));
    for(uint16_t m = 0; m < amount; m++)
    {

        DMA_WS2812_data_shift_light(amount - m, amount);
        DMA_WS2812_Reset();
        HAL_Delay(1);
        DMA_WS2812_light(amount);
        HAL_Delay(20);
    }
    memset(ws28128_color_buf, 0, sizeof(ws28128_color_buf));

//     DMA_WS2812_Mie(amount);

}
void DMA_WS2812_Ramp(volatile uint16_t  amount, uint8_t pwm, colors_kind color)
{
    for(uint16_t m = 0; m < pwm; m++)
    {

        DMA_WS2812_SIN(amount, m, color);
        DMA_WS2812_Reset();
        HAL_Delay(10);
        DMA_WS2812_light(amount);
        HAL_Delay(10);


    }

}
/***********多条灯跑起来****************/
void DMA_WS2812_Running_more(volatile uint16_t  amount, volatile uint16_t run_number)
{
    for(uint16_t m = 0; m < amount; m++)
    {


        DMA_WS2812_data_shift_more(m, 10);
        DMA_WS2812_Reset();
        HAL_Delay(1);
        DMA_WS2812_light(amount);
        HAL_Delay(30);
		} 

    for(uint16_t m = 0; m < amount; m++)
    {

        DMA_WS2812_data_shift_more(amount - m, 10);
        DMA_WS2812_Reset();
        HAL_Delay(1);
        DMA_WS2812_light(amount);
        HAL_Delay(30);
		}
}

