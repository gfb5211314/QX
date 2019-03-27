#include   "bsp_ws2812.h"
#include "stm32f1xx_hal.h"
#include  "math.h"
#include   "bsp_dma.h"
#include "string.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern DMA_HandleTypeDef hdma_tim2_ch1;


#define  HTIM         htim2
#define  CHANAL       TIM_CHANNEL_1
#define   DMA_TIM_CH   hdma_tim2_ch1
/****  Macro definition parameter  ****************/

#define BIT_1                      61u //1比较值为61 850us
#define BIT_0                      29u //0比较值为28 400us 
#define  LED_NUM                   24*10

/* Private variables ---------------------------------------------------------*/
uint16_t ab[LED_NUM];
uint16_t reset_buf[200];
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


#define  reset_max        50

uint8_t    ws2812_state = 0;
uint16_t   ws28128_reset_buf[50] = {0}; //62.5us      大于50us低电平复位
uint16_t   ws28128_end_reset_buf[50] = {0};
uint16_t   ws28128_mie_buf[48] = {28};
uint8_t   ws28128_color_buf[256][3];

//接收colorbuf
uint16_t   ws28128_color_data[48]= {30, 30, 30, 30, 30, 30, 60, 60,
                                     30, 30, 30, 30, 30, 30, 30, 30,
                                     30, 30, 30, 30, 30, 30, 30, 30,
                                     30, 30, 30, 30, 30, 30, 30, 30,
                                     30, 30, 30, 30, 30, 30, 30, 60,
                                     30, 30, 30, 30, 30, 60, 30, 30,
                                    };   //PWM数据



uint8_t   Shift_mode = 0;
/*******************/
uint8_t   reset_flag = 0;
uint8_t   send_flag = 0;
uint8_t  updata_flag = 0;
uint8_t   mie_flag = 0;
uint16_t   mie_led_count;
uint16_t   mie_led_max;
volatile uint8_t half_tx ;
volatile uint8_t full_tx ;
volatile uint8_t reset_end_flag = 0;
extern   uint8_t  systick_500ms_flag;
extern uint16_t  k3;

uint16_t light_led_max;
uint8_t  light_flag;
uint16_t light_led_count;
/******注册完成回调不行************/
/*DMA传输一半回调*/

/**
  * @brief   Half transfer callback function of DMA
  * @param  DMA_HandleTypeDef: DMA HAND
  * @retval None
  */
void DMA_HalfCpltCallback(DMA_HandleTypeDef* _hdma)
{


}

/*DMA传输完成回调*/

/**
  * @brief  transfer complete callback function of DMA
  * @param  DMA_HandleTypeDef: DMA HAND
  * @retval None
  */
void DMA_CpltCallback(DMA_HandleTypeDef* _hdma)
{


}
/*注册DMA传输一半回调中断函数**********/
/**
  * @brief  Register  Half transfer callback function of DMA
  * @retval None
  */

void DMA_RegisterHalfCallback()
{
    HAL_DMA_RegisterCallback(&DMA_TIM_CH, HAL_DMA_XFER_HALFCPLT_CB_ID, DMA_HalfCpltCallback);
}
/*注册DMA传输完成回调中断函数**********/
/**
  * @brief  Register  transfer complete  callback function of DMA
  * @retval None
  */

void DMA_RegisterCpltCallback()
{
    HAL_DMA_RegisterCallback(&DMA_TIM_CH, HAL_DMA_XFER_CPLT_CB_ID, DMA_CpltCallback);
}
/**
  * @brief  intitialize callback function of DMA
  * @retval None
  */
void DMA_Callback_init()
{
    DMA_RegisterHalfCallback();
    DMA_RegisterCpltCallback();

}
/**
  * @brief  Convert RGB data to GRB
  * @param  count : number of led  0-6556
  * @param  uint8_t p :  An array of converted data
  * @retval None  
  */

void  RGBTOGRB(uint16_t count, uint8_t p[][3])
{
    uint8_t tem_data;
    for(uint16_t i = 0; i < count; i++)
    {
        tem_data = p[i][0];;
        p[i][0] = p[i][1];
        p[i][1] = tem_data;
        p[i][2] = p[i][2];


    }



}
/**
  * @brief  Shift the array's LED data to the 0 position of the array
  * @param  led_location :  The location of the LED
  * @param  uint8_t p :  An array of converted data
  * @retval None  
  */
void shift_buf_to_data_one(uint16_t led_location)
{
    uint16_t gree_data, red_data, blue_data;
	/*Prevents the original data from being shifted to 0*/
    gree_data = ws28128_color_buf[led_location][1];  
    red_data = ws28128_color_buf[led_location][0];
    blue_data = ws28128_color_buf[led_location][2];
    for(uint16_t i = 0; i < 24; i++)
    {
        if(i < 8)
        {

            if(gree_data & 0x80)
            {
                ws28128_color_data[i] = BIT_1; //1

            }
            else
            {
                ws28128_color_data[i] = BIT_0; //0
            }
            gree_data = gree_data << 1;

        }
        else if((7 < i) && (i < 16))
        {

            if(red_data & 0x80)
            {
                ws28128_color_data[i] = BIT_1; //1

            }
            else
            {
                ws28128_color_data[i] = BIT_0; //0
            }
            red_data = red_data << 1;


        }
        else
        {
            if(blue_data & 0x80)
            {
                ws28128_color_data[i] = BIT_1; //1

            }
            else
            {
                ws28128_color_data[i] = BIT_0; //0
            }
            blue_data = blue_data << 1;


        }


    }

}
/**
  * @brief  Move the array's LED data to position 1 of the array 
  * @param  led_location :  The location of the LED
  * @param  uint8_t p :  An array of converted data
  * @retval None  
  */
void shift_buf_to_data_two(uint16_t led_location)
{
    uint16_t gree_data, red_data, blue_data;
    gree_data = ws28128_color_buf[led_location][1];
    red_data = ws28128_color_buf[led_location][0];
    blue_data = ws28128_color_buf[led_location][2];
    for(uint16_t i = 24; i < 48; i++)
    {
        if(i < 32)
        {

            if(gree_data & 0x80)
            {
                ws28128_color_data[i] = BIT_1; //1

            }
            else
            {
                ws28128_color_data[i] = BIT_0; //0
            }
            gree_data = gree_data << 1;

        }
        else if((31 < i) && (i < 40))
        {

            if(red_data & 0x80)
            {
                ws28128_color_data[i] = BIT_1; //1

            }
            else
            {
                ws28128_color_data[i] = BIT_0; //0
            }
            red_data = red_data << 1;


        }
        else
        {
            if(blue_data & 0x80)
            {
                ws28128_color_data[i] = BIT_1; //1

            }
            else
            {
                ws28128_color_data[i] = BIT_0; //0
            }
            blue_data = blue_data << 1;


        }


    }

}

/**
  * @brief  Convert GRB data into PWM arrays
  * @param  led_location :  number of led
  * @retval None  
  */


void GRBTOPWM(uint16_t led_max)
{
    uint8_t    gree_data;
    uint8_t    blue_data;
    uint8_t    red_data;
    for(uint16_t j = 0; j < led_max; j++)
    {
        gree_data = ws28128_color_buf[j][1];
        red_data = ws28128_color_buf[j][0];
        blue_data = ws28128_color_buf[j][2];
        for(uint16_t i = 0; i < 24; i++)
        {

            if(i < 8)
            {

                if(gree_data & 0x80)
                {
                    ws28128_color_data[i + (j * 24)] = BIT_1; //1

                }
                else
                {
                    ws28128_color_data[i + (j * 24)] = BIT_0; //0
                }
                gree_data = gree_data << 1;

            }
            else if((7 < i) && (i < 16))
            {

                if(red_data & 0x80)
                {
                    ws28128_color_data[i + (j * 24)] = BIT_1; //1

                }
                else
                {
                    ws28128_color_data[i + (j * 24)] = BIT_0; //0
                }
                red_data = red_data << 1;


            }
            else
            {
                if(blue_data & 0x80)
                {
                    ws28128_color_data[i + (j * 24)] = BIT_1; //1

                }
                else
                {
                    ws28128_color_data[i + (j * 24)] = BIT_0; //0
                }
                blue_data = blue_data << 1;


            }


        }
    }
}

void ws2812_init()
{


}
void  SET_PWM_DMA_DODE(uint32_t mode)
{

    __HAL_RCC_TIM2_CLK_ENABLE();
  
    /* TIM2 DMA Init */
    /* TIM2_CH1 Init */
    hdma_tim2_ch1.Instance = DMA1_Channel5;
    hdma_tim2_ch1.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_tim2_ch1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_tim2_ch1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_tim2_ch1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_tim2_ch1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_tim2_ch1.Init.Mode = mode;
    hdma_tim2_ch1.Init.Priority = DMA_PRIORITY_MEDIUM;
    if (HAL_DMA_Init(&hdma_tim2_ch1) != HAL_OK)
    {
      _Error_Handler(__FILE__, __LINE__);
    }

    __HAL_LINKDMA(&HTIM,hdma[TIM_DMA_ID_CC1],hdma_tim2_ch1);

    /* Prepare the DMA Channel configuration */


    /* Write to DMA Channel CR register */


}








 
/********************************************************************
       以下函数采用半传输中断更新数据，优化的底层函数和应用函数
                    BOTTOM function
               *DMA_WS2812_Reset              ws2812复位
               *DMA_WS2812_light              ws2812亮
               *DMA_WS2812_Mie                ws2812灭
               *DMA_WS2812_SIN                正弦函数
                     APP
               *DMA_WS2812_Ramp                渐变效果一会灭一会亮,采用正弦
               *DMA_WS2812_Run                 依次亮(每个都亮，每个灯位置颜色不同)
               *DMA_WS2812_Running             几个跑(亮度相同,几个跑起来)
               *DMA_WS2812_Ramping             同一类颜色渐变 (深红,浅红)

**********************************************************************/
/********复位************/
void DMA_WS2812_Reset()
{

    HAL_TIM_PWM_Stop_DMA(&HTIM, TIM_CHANNEL_1);
    __HAL_RCC_DMA1_CLK_ENABLE();
    SET_PWM_DMA_DODE(DMA_NORMAL);
    reset_flag = 1;
    HAL_TIM_PWM_Start_DMA(&HTIM, TIM_CHANNEL_1, (uint32_t*)ws28128_reset_buf, reset_max);

}
void DMA_WS2812_light(uint16_t led_n)
{
    light_led_max = led_n;
    HAL_TIM_PWM_Stop_DMA(&HTIM, TIM_CHANNEL_1);
    __HAL_RCC_DMA1_CLK_ENABLE();
    SET_PWM_DMA_DODE(DMA_CIRCULAR);
    DMA_Callback_init();
    GRBTOPWM(2);
    light_flag = 1;
    light_led_count = 1;
    HAL_TIM_PWM_Start_DMA(&HTIM, TIM_CHANNEL_1, (uint32_t*)ws28128_color_data, 48);
}
void DMA_WS2812_Mie(uint16_t led_n)
{
    mie_led_max = led_n;
    HAL_TIM_PWM_Stop_DMA(&HTIM, TIM_CHANNEL_1);
    __HAL_RCC_DMA1_CLK_ENABLE();
    SET_PWM_DMA_DODE(DMA_CIRCULAR);
    DMA_Callback_init();
    mie_flag = 1;
    mie_led_count = 0;
    for(uint16_t i = 0; i < 48; i++)
    {
        ws28128_mie_buf[i] = 29;
    }
    HAL_TIM_PWM_Start_DMA(&HTIM, TIM_CHANNEL_1, (uint32_t*)ws28128_mie_buf, 48);
}
/**********数据的值*****************/
void DMA_WS2812_SIN(uint16_t amount, uint8_t pwm, colors_kind color)
{
    uint8_t     sin_y = 0;
//    printf("START");

    uint8_t point;
    float sin_x;  //弧度
    float sin_jiao;//角度 分度角
    sin_jiao = 360.000 / 255;
    point = pwm;

    sin_x = sin_jiao * pwm; //得到角度值
    sin_x = sin_x * 0.01744; //角度转弧度 ?弧度=角度*（π/180）
    sin_y = 128 * sin(sin_x + 4.75) + 128;

    for(uint16_t j = 0; j < amount; j++)
    {


        if(color == green)
        {
            ws28128_color_buf[j][0] = 0; //red
            ws28128_color_buf[j][1] = sin_y; //gree
            ws28128_color_buf[j][2] = 0;	 //blue
        }
        else if(color == red)
        {
            ws28128_color_buf[j][0] = sin_y; //red
            ws28128_color_buf[j][1] = 0; //gree
            ws28128_color_buf[j][2] = 0;	 //blue
        }
        else if(color == blue)
        {
            ws28128_color_buf[j][0] = 0; //red
            ws28128_color_buf[j][1] = 0; //gree
            ws28128_color_buf[j][2] = sin_y;	 //blue
        }
//         else if(color == yellow)
//        {
//            ws28128_color_buf[j][0] = sin_y; //red
//            ws28128_color_buf[j][1] = sin_y; //gree
//            ws28128_color_buf[j][2] = 0;	 //blue
//        }
    }

}
/***灭数据移位************/
void DMA_WS2812_data_shift(uint16_t led_location)
{
	
	        memset(ws28128_color_buf, 0, sizeof(ws28128_color_buf));
	          ws28128_color_buf[led_location+1][0] = 255; //gree
          	ws28128_color_buf[led_location+2][1] = 255; //red
            ws28128_color_buf[led_location][2] = 255;	 //blue
	          ws28128_color_buf[led_location+3][0] = 255;	 //blue
	          ws28128_color_buf[led_location+3][1] = 255;	 //blue
           
	        
       
	
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



 /***全亮数据移位,第几个位置灯的颜色，由我设置**/
/***配色函数***********************/
void DMA_WS2812_data_shift_light(uint16_t led_location,uint16_t led_max)
{
	              
	          if(led_location<(led_max/4))  //gree
						{
	          ws28128_color_buf[led_location][0] = 0; //R
          	ws28128_color_buf[led_location][1] = 2; //G
            ws28128_color_buf[led_location][2] = 0; //blue
						}
						else if(led_location>(led_max/4-1)&&(led_location<(led_max/2)) ) //blue
						{
            ws28128_color_buf[led_location][0] = 0; //R
          	ws28128_color_buf[led_location][1] = 0; //G
            ws28128_color_buf[led_location][2] = 2; //blue
							
						}
								else if(led_location>(led_max/2-1)&&(led_location<led_max ) )//blue
						{
            ws28128_color_buf[led_location][0] = 0; //R
          	ws28128_color_buf[led_location][1] = 255; //G
            ws28128_color_buf[led_location][2] = 255; //blue
							
						}
					
    
}

void DMA_WS2812_Run(volatile uint16_t  amount)
{
	 for(uint16_t m = 0; m < amount; m++)
    {

       DMA_WS2812_data_shift_light(m,amount);
        DMA_WS2812_Reset();
        HAL_Delay(10);
        DMA_WS2812_light(amount);
        HAL_Delay(30);
		}
		memset(ws28128_color_buf, 0, sizeof(ws28128_color_buf));
     
     DMA_WS2812_Mie(amount);
	  
}
void DMA_WS2812_Running(volatile uint16_t  amount)
{
	 for(uint16_t m = 0; m < amount; m++)
    {

       DMA_WS2812_data_shift(m);
        DMA_WS2812_Reset();
        HAL_Delay(10);
        DMA_WS2812_light(amount);
        HAL_Delay(30);
		}



}	






