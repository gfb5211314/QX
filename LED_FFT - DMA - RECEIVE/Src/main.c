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
#include "bsp_fft.h"
#include "string.h"
#include "bsp_ws2812.h"
#include "bsp_usart.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
DMA_HandleTypeDef hdma_tim2_ch1;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
/*Define print function*/

extern USART_RECEIVETYPE  UsartType2;
extern USART_RECEIVETYPE  UsartType3;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

                                

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
 void SystemClock_Config(void);
 void MX_GPIO_Init(void);
 void MX_DMA_Init(void);
 void MX_TIM3_Init(void);
 void MX_USART1_UART_Init(void);
 void MX_ADC1_Init(void);
 void MX_TIM2_Init(void);
 void MX_USART2_UART_Init(void);
 void MX_NVIC_Init(void);
 void MX_USART3_UART_Init();              
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */


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
uint8_t CON_FALG_count;
uint16_t  uhADCxConvertedValue1[TEST_LENGTH_SAMPLES/2]; //改成uint32_t不行，奇怪
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    /* Prevent unused argument(s) compilation warning */

//     CON_FALG
// if(__HAL_DMA_GET_FLAG(hdma_adc1, DMA_FLAG_TC1))
//	{
//	     HAL_ADC_Stop_DMA(&hadc1);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */
	
//	}
    /* NOTE : This function should not be modified. When the callback is needed,
              function HAL_ADC_ConvCpltCallback must be implemented in the user file.
     */
}
void DMA1_Channel1_IRQHandler(void)
{
	 if(__HAL_DMA_GET_FLAG(hdma_adc1, DMA_FLAG_TC1))
	{
	CON_FALG=1;
HAL_ADC_Stop_DMA(&hadc1);
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

    }
  
		 


  /* USER CODE END DMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc1);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

  /* USER CODE END DMA1_Channel1_IRQn 1 */
}
extern uint8_t light_flag;
extern uint8_t  reset_flag;
extern uint8_t reset_count_flag;
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
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();

  /* USER CODE BEGIN 2 */
	DMA_USART_IDLE_INIT();
	DMA_WS2812_Reset();
//  HAL_TIM_Base_Start(&htim3);
//HAL_ADC_Start_DMA(&hadc1, (uint32_t*)uhADCxConvertedValue1, (TEST_LENGTH_SAMPLES/2));
//
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while(1)
    {
			
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
//			if(CON_FALG==1)
//			{
//	      CON_FALG=0;
////	    	DMA_WS2812_FFT_DSPALY(32,8,2,testOutput);
//			
//       HAL_ADC_Start_DMA(&hadc1, (uint32_t*)uhADCxConvertedValue1, (TEST_LENGTH_SAMPLES/2));
//	    }

//		 else if((CON_FALG==0)&&
     if(UsartType2.RX_flag==1)
		 {
  		 
//       if(reset_count_flag==1)
//			 {
//				 reset_count_flag=0;
		 DMA_WS2812_Reset();
			         
      HAL_Delay(1);
      DMA_WS2812_light(256);
	    HAL_Delay(1);
			 
//
			 UsartType2.RX_flag=0;
		 }
			 
			 
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
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Enables the Clock Security System 
    */
  HAL_RCC_EnableCSS();

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

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
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
  /* ADC1_2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(ADC1_2_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
  /* DMA1_Channel6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
  /* USART3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART3_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USART2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART2_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* DMA1_Channel3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
}












/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
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
