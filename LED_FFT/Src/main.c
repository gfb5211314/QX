/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"

/* USER CODE BEGIN Includes */
#include "arm_math.h"
#include "arm_const_structs.h"
#include "bsp_tim.h"
#include "bsp_led.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
DMA_HandleTypeDef hdma_tim2_ch1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
/*Define print function*/

int fputc(int ch, FILE* f)
{
    HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, 0xffff);
    return ch;
}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM2_Init(void);
static void MX_NVIC_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim);


/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void MX_TIM2_Init(void);
void MX_TIM3_Init(void);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
//#define NPT  64  //1024?FFT
//#define Fs 1000  //采样频率
//#define PI2 6.28318530717959
//// float32_t  testInput_f32[NPT];
//// float32_t  testOutput_f32[NPT];
//// float32_t  testOutput[NPT];


//#define TEST_LENGTH_SAMPLES 1024

///* 输入和输出缓冲 */
//static float32_t testOutput[TEST_LENGTH_SAMPLES/2];
//static float32_t testInput_f32_10khz[TEST_LENGTH_SAMPLES];

///* 变量 */
////uint32_t fftSize = 32;
//uint32_t ifftFlag = 0;  //正向
//uint32_t doBitReverse = 1; // 启动输出位反转
//const static arm_cfft_instance_f32 arm_cfft_sR_f32_len1024;
//void arm_rfft_fast_f32_app(void)
//{
//  uint16_t i;

//

//

// for(i=0; i<1024; i++)
//  {
//    /*3种频率 50Hz 2500Hz 2550Hz */
//    testInput_f32_10khz[i] = 1000*arm_sin_f32(PI2*i*50.0/Fs) +
//               2000*arm_sin_f32(PI2*i*2500.0/Fs)  +
//               3000*arm_sin_f32(PI2*i*2550.0/Fs);
//  }



//   /* CFFT变换 */
//     arm_cfft_f32(&arm_cfft_sR_f32_len1024, testInput_f32_10khz, ifftFlag, doBitReverse);

//     /* 求解模值  */
//     arm_cmplx_mag_f32(testInput_f32_10khz, testOutput, 1024);

//  for(i=0; i<512; i++)
//  {
//    printf("%f\r\n", testOutput[i]);
//  }
//}


/* -------------------------------------------------------------------- */

#define TEST_LENGTH_SAMPLES 1024

/* -------------------------------------------------------------------
* External Input and Output buffer Declarations for FFT Bin Example
* ------------------------------------------------------------------- */
static float32_t testInput_f32_10khz[TEST_LENGTH_SAMPLES];
static float32_t testOutput[TEST_LENGTH_SAMPLES / 2];
/* ------------------------------------------------------------------
* Global variables for FFT Bin Example
* ------------------------------------------------------------------- */
static uint32_t fftSize = TEST_LENGTH_SAMPLES/2;
uint32_t ifftFlag = 0;
uint32_t doBitReverse = 1;
/* Reference index at which max energy of bin ocuurs */

uint8_t  CON_FALG=0;
 uint16_t  uhADCxConvertedValue1[TEST_LENGTH_SAMPLES/2];
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    /* Prevent unused argument(s) compilation warning */
   // HAL_ADC_Stop_DMA(&hadc1);
 
    for(uint16_t i = 0; i < (TEST_LENGTH_SAMPLES/2); i++)
    {
        /* 虚部全部置零 */
      testInput_f32_10khz[i * 2 + 1] = 0;

        /* ADC的值作为实部 */
        testInput_f32_10khz[i*2] = uhADCxConvertedValue1[i];

    }
  
		  arm_cfft_f32(&arm_cfft_sR_f32_len512, testInput_f32_10khz, ifftFlag, doBitReverse);
  /* Process the data through the Complex Magnitude Module for
  calculating the magnitude at each bin */
    arm_cmplx_mag_f32(testInput_f32_10khz, testOutput, fftSize);
			  for(uint16_t i=0;i<(TEST_LENGTH_SAMPLES/2);i++)
		{
			printf("%d=%f\r\n", i,testOutput[i]);
		
		}
		 //CON_FALG++;
	
		 led_fft(28,8,1,3,testOutput);
		  
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)uhADCxConvertedValue1,( TEST_LENGTH_SAMPLES/2));
    /* NOTE : This function should not be modified. When the callback is needed,
              function HAL_ADC_ConvCpltCallback must be implemented in the user file.
     */
}
//void ADC_DMAConvCplt(DMA_HandleTypeDef *hdma)
//{
//	 printf("DMA\r\n");
//
//
//}
/* USER CODE END 0 */

int main(void)
{

    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration----------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_TIM3_Init();
    MX_USART1_UART_Init();
    MX_ADC1_Init();
    MX_TIM2_Init();

    /* Initialize interrupts */
    MX_NVIC_Init();

    /* USER CODE BEGIN 2 */

    /* 按照实部，虚部，实部，虚部..... 的顺序存储数据 */
//     for(uint16_t i=0; i<1024; i++)
//     {
//         /* 虚部全部置零 */
//        testInput_f32_10khz[i*2+1] = 0;
//
//         /* 50Hz正弦波，采样率1KHz ,作为实部 */
//         testInput_f32_10khz[i*2] = arm_sin_f32(2*3.1415926f*50*i/1000)+arm_sin_f32(2*3.1415926f*50*i/1000);
//     }
    /* Process the data through the CFFT/CIFFT module */

    /* Calculates maxValue and returns corresponding BIN value */
//  arm_max_f32(testOutput, fftSize, &maxValue, &testIndex);
//  if (testIndex !=  refIndex)
//  {
//    status = ARM_MATH_TEST_FAILURE;
//  }
//	 if ( status != ARM_MATH_SUCCESS)
//  {
//    while (1);
//  }
//		  for(uint16_t i=0;i<1024;i++)
//		{
//			printf("%f\r\n", testOutput[i]);
//
//		}

	 
  // led_PWM(1, 1, 2);
 HAL_TIM_Base_Start(&htim3);
	//HAL_TIM_Base_Start_IT(&htim2);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)uhADCxConvertedValue1, (TEST_LENGTH_SAMPLES/2));
    /* USER CODE END 2 */
    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while(1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
//		if(CON_FALG==1)
//		{
//   arm_cfft_f32(&arm_cfft_sR_f32_len512, testInput_f32_10khz, ifftFlag, doBitReverse);
//  /* Process the data through the Complex Magnitude Module for
//  calculating the magnitude at each bin */
//    arm_cmplx_mag_f32(testInput_f32_10khz, testOutput, fftSize);

////			  for(uint16_t i=0;i<(TEST_LENGTH_SAMPLES/2);i++)
////		{
////			printf("%f\r\n", testOutput[i]);
////		
////		}
//		CON_FALG=0;
//		}
  
	
		
 //  led_fft(5,8,1,5,testOutput);
     //  led_runing(80, 1, 2, 2000);
//        led_runing(2, 2, 2, 2000);
    }
    /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    /**Initializes the CPU, AHB and APB busses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
    if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    /**Enables the Clock Security System
    */
    HAL_RCC_EnableCSS();

    /**Configure the Systick interrupt time
    */
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

    /**Configure the Systick
    */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/** NVIC Configuration
*/
static void MX_NVIC_Init(void)
{
    /* DMA1_Channel1_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
    /* DMA1_Channel5_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
    /* ADC1_2_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(ADC1_2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
    /* TIM2_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
    /* TIM3_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
}

/* ADC1 init function */
static void MX_ADC1_Init(void)
{

    ADC_ChannelConfTypeDef sConfig;

    /**Common config
    */
    hadc1.Instance = ADC1;
    hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc1.Init.ContinuousConvMode = ENABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    if(HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    /**Configure Regular Channel
    */
    sConfig.Channel = ADC_CHANNEL_0;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
    if(HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

}

/* TIM2 init function */
static void MX_TIM2_Init(void)
{

    TIM_ClockConfigTypeDef sClockSourceConfig;
    TIM_MasterConfigTypeDef sMasterConfig;
    TIM_OC_InitTypeDef sConfigOC;

    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 0;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 89;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if(HAL_TIM_Base_Init(&htim2) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if(HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    if(HAL_TIM_PWM_Init(&htim2) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if(HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    if(HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    HAL_TIM_MspPostInit(&htim2);

}



/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if(HAL_UART_Init(&huart1) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{
    /* DMA controller clock enable */
    __HAL_RCC_DMA1_CLK_ENABLE();

}

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char* file, int line)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    while(1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
      ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */

}

#endif

/**
  * @}
  */

/**
  * @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
