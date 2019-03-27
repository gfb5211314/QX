
/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  FuBiao.Guo
  * @version V1.1.0
  * @date    25-2-2019
  * @brief   Lamp underlying driver function file.
	**/


#include "stm32f1xx_hal.h"
#include "bsp_usart.h"
#include "bsp_led.h"

extern TIM_HandleTypeDef htim2;






/****  Macro definition parameter  ****************/
#define lamp_bead_amount           1000            //lamp bead amount 
#define BIT_1                      61u //1比较值为61 850us
#define BIT_0                      28u //0比较值为28 400us 


/* Private variables ---------------------------------------------------------*/
uint16_t ab[1000];
uint16_t reset[200];
color_type    color_buf[35] = { {0xff, 0x00, 0x00}, //绿色
    {0x00, 0xff, 0x00}, //红色
    {0x00, 0x00, 0xff}, //蓝色
    {255, 255, 31}, //黄色
    {238, 118, 198},
    { 255, 84,  159},
    { 191,  0, 255},
    { 164, 244,  96},
    {0, 51, 51}
};
color_type    color_mie[35] = { 0 };

/**
 * @brief  Control the color of single LED.
 * @param  amount :   Maximum number of lighting leds
 * @param  color  :    The color of the light
 * @param  pwm    :    The brightness of the light (1-256)
 * @retval None
 */
void  led_run(volatile uint16_t  amount, colors_kind  color, volatile uint8_t pwm)
{
    uint16_t  count = 0;
    volatile	uint16_t    amount_size = 0;
    uint16_t   LED_count = 0;
    volatile    uint8_t  pwm_1 = 0;
	  amount_size = amount * 24 + 1;
    pwm_1 = pwm;

    for(LED_count = 0; LED_count < amount; LED_count++)
    {

        pwm = pwm_1;
        for(count = 0; count < 24; count++)
        {
            //绿色
            if(color == 1)
            {
                if(count < 8)
                {

                    if(pwm & 0x80)
                    {
                        ab[(count + (LED_count * 24))] = 60; //1

                    }
                    else
                    {
                        ab[(count + (LED_count * 24))] = 30; //0
                    }
                    pwm = pwm << 1;

                }
                else
                {


                    ab[(count + (LED_count * 24))] = 30;


                }
            }
            else if(color == 2)
            {

                if(count < 8)
                {
                    ab[(count + (LED_count * 24))] = 30;

                }
                else if((7 < count) && (count < 16))
                {
                    if(pwm & 0x80)
                    {
                        ab[(count + (LED_count * 24))] = 60; //1

                    }
                    else
                    {
                        ab[(count + (LED_count * 24))] = 30; //0
                    }
                    pwm = pwm << 1;
                }
                else
                {


                    ab[(count + (LED_count * 24))] = 30; //0

                }
            }
            else if(color == 3)
            {
                if(count < 16)
                {

                    ab[(count + (LED_count * 24))] = 30; //0

                }
                else
                {


                    if(pwm & 0x80)
                    {
                        ab[(count + (LED_count * 24))] = 60; //1

                    }
                    else
                    {
                        ab[(count + (LED_count * 24))] = 30; //0
                    }
                    pwm = pwm << 1;

                }



            }
            else if(color == 4)
            {

                ab[(count + (LED_count * 24))] = 30; //0








            }
        }
        if(LED_count == (amount - 1))
        {
            ab[amount * 24 + 1] = 0;
        }
    }

    HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t*)ab, amount_size);

}




/**
 * @brief  Control single LED flashing.
 * @param  amount :   Maximum number of lighting leds
 * @param  color  :    The color of the light
 * @param  pwm    :    The brightness of the light (1-256)
 * @param  time    :    Flicker time size(ms)
 * @retval None
 */
void  led_runing(volatile uint16_t  amount, colors_kind  color, volatile uint8_t pwm, uint32_t time)
{
    led_run(35, 4, pwm);
    HAL_Delay(time);
    led_run(amount, color, pwm);
    HAL_Delay(time);


}
/**
 * @brief  Light up the led in turn, and the color gradually brightens.
 * @param  amount :   Maximum number of lighting leds
 * @param  color  :    The color of the light
 * @param  pwm    :    Brightness level (1-256)
 * @retval None
 */

void  led_open(volatile uint16_t  amount, colors_kind  color, volatile uint8_t pwm)
{
    uint16_t  count = 0;
    volatile	uint16_t    amount_size = 0;
    uint8_t  bright;
    amount_size = amount * 24 + 1;
    bright = (pwm / amount);
    uint16_t   LED_count = 0;
    volatile    uint8_t  pwm_1 = 0;
    pwm_1 = pwm;

    for(LED_count = 0; LED_count < amount; LED_count++)
    {

        pwm = bright * (LED_count + 1);
        for(count = 0; count < 24; count++)
        {
            //绿色
            if(color == 1)
            {
                if(count < 8)
                {

                    if(pwm & 0x80)
                    {
                        ab[(count + (LED_count * 24))] = 60; //1

                    }
                    else
                    {
                        ab[(count + (LED_count * 24))] = 30; //0
                    }
                    pwm = pwm << 1;

                }
                else
                {


                    ab[(count + (LED_count * 24))] = 30;


                }
            }
            //红色
            else if(color == 2)
            {

                if(count < 8)
                {
                    ab[(count + (LED_count * 24))] = 30;

                }
                else if((7 < count) && (count < 16))
                {
                    if(pwm & 0x80)
                    {
                        ab[(count + (LED_count * 24))] = 60; //1

                    }
                    else
                    {
                        ab[(count + (LED_count * 24))] = 30; //0
                    }
                    pwm = pwm << 1;
                }
                else
                {


                    ab[(count + (LED_count * 24))] = 30; //0

                }
            }
            //蓝色
            else if(color == 3)
            {
                if(count < 16)
                {

                    ab[(count + (LED_count * 24))] = 30; //0

                }
                else
                {


                    if(pwm & 0x80)
                    {
                        ab[(count + (LED_count * 24))] = 60; //1

                    }
                    else
                    {
                        ab[(count + (LED_count * 24))] = 30; //0
                    }
                    pwm = pwm << 1;

                }



            }
            //灭灯
            else if(color == 4)
            {

                ab[(count + (LED_count * 24))] = 30; //0








            }
            //蓝绿色
            else if(color == 5)
            {

                if(count < 8)
                {
                    ab[(count + (LED_count * 24))] = 60; //1

                }
                else if((7 < count) && (count < 16))
                {

                    ab[(count + (LED_count * 24))] = 30; //0


                }
                else
                {


                    ab[(count + (LED_count * 24))] = 60; //255

                }
            }
            //黄色
            else if(color == 6)
            {

                if(count < 8)
                {
                    ab[(count + (LED_count * 24))] = 60; //

                }
                else if((7 < count) && (count < 16))
                {

                    ab[(count + (LED_count * 24))] = 60; //


                }
                else
                {


                    ab[(count + (LED_count * 24))] = 30; //

                }
            }

        }
        if(LED_count == (amount - 1))
        {
            ab[amount * 24 + 1] = 0;
        }
    }

    HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t*)ab, amount_size);

}


/**
 * @brief  Light up the led in turn, and the color gradually brightens.
 * @param  amount :   Maximum number of lighting leds
 * @param  color  :    The color of the light
 * @param  pwm    :    Brightness level (1-256)
 * @param  time   :    Turn on the light up time(ms)
 * @retval None
 */

void  led_opening(volatile uint16_t  amount, colors_kind  color, volatile uint8_t pwm, uint32_t time)
{

    for(uint16_t i = 1; i < (amount + 1); i++)
    {
        HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t*)reset, 200);
        HAL_Delay(1);
        led_open(i, color, 250);
        HAL_Delay(time);
    }

}




/**
 * @brief  Light color type.
 * @param  amount :   Maximum number of lighting leds
 * @param  p      :    Color combination array
 * @retval None
 */
void  led_type(volatile uint16_t  amount,  color_type* p)
{

    uint16_t amount_size = 0;
    uint16_t n = 0;
    color_type p_tem[amount];

    for(n = 0; n < amount; n++)
    {
        p_tem[n].gree = p[n].gree;
        p_tem[n].red = p[n].red;
        p_tem[n].blue = p[n].blue;


    }
    amount_size = amount * 24 + 1;

    for(uint16_t i = 0; i < amount; i++)
    {
//			 p[i].greed=0;
//			 p[i].red=0;
//			 p[i].blue=0;
        printf("%d.gree=%d\r\n", i, p[i].gree);
        printf("%d.red=%d\r\n", i, p[i].red);
        printf("%d.blue=%d\r\n", i, p[i].blue);
        for(uint16_t j = 0; j < 24; j++)
        {
            if(j < 8)
            {
                if(p_tem[i].gree & 0x80)
                {
                    ab[(j + (i * 24))] = 60; //1

                }
                else
                {
                    ab[(j + (i * 24))] = 30; //0
                }
                p_tem[i].gree = p_tem[i].gree << 1;
            }
            else if((7 < j) && (j < 16))
            {
                if(p_tem[i].red & 0x80)
                {
                    ab[(j + (i * 24))] = 60; //1

                }
                else
                {
                    ab[(j + (i * 24))] = 30; //0
                }
                p_tem[i].red = p_tem[i].red << 1;


            }
            else
            {
                if(p_tem[i].blue & 0x80)
                {
                    ab[(j + (i * 24))] = 60; //1

                }
                else
                {
                    ab[(j + (i * 24))] = 30; //0
                }
                p_tem[i].blue = p_tem[i].blue << 1;

            }
        }


        if(i == (amount - 1))
        {
            ab[amount * 24] = 0;
        }


    }

    HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t*)ab, amount_size);

}