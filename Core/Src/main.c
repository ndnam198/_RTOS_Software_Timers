/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include "timers.h"
#include "myLib.h"
#include "myRTOSaddons.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ONESHOT_TIMER_3 (pdMS_TO_TICKS(3333))
#define TIMER_1_FREQ (pdMS_TO_TICKS(50))
#define TIMER_2_FREQ (pdMS_TO_TICKS(333))

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* Definitions for blinkLed1 */
osThreadId_t blinkLed1Handle;
const osThreadAttr_t blinkLed1_attributes =
    {
        .name = "blinkLed1",
        .priority = (osPriority_t)osPriorityLow4,
        .stack_size = 64 * 4};
/* Definitions for software timers */

TimerHandle_t PeriodicTimer1Handle;
TimerHandle_t PeriodicTimer2Handle;
TimerHandle_t OneShotTimer3Handle;

/* USER CODE BEGIN PV */
uint32_t error_count = 0;
static char main_string[10];
const uint32_t TIMER_1_ID = 1;
const uint32_t TIMER_2_ID = 2;
const uint32_t TIMER_3_ID = 3;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);

void blinkTask(void *argument);
void timer1Callback(void *argument);
void timer2Callback(void *argument);
void timer3Callback(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  offAllLed;
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of PeriodicTimer1 */
  PeriodicTimer1Handle = xTimerCreate("PeriodicTimer1", TIMER_1_FREQ,
                                      pdTRUE, 0, timer1Callback);
  PeriodicTimer2Handle = xTimerCreate("PeriodicTimer2", TIMER_2_FREQ,
                                      pdTRUE, 0, timer2Callback);
  OneShotTimer3Handle = xTimerCreate("OneShotTimer3", ONESHOT_TIMER_3,
                                     pdFALSE, 0, timer3Callback);
  vTimerSetTimerID(PeriodicTimer1Handle, (void *)TIMER_1_ID);
  vTimerSetTimerID(PeriodicTimer2Handle, (void *)TIMER_2_ID);
  vTimerSetTimerID(OneShotTimer3Handle, (void *)TIMER_3_ID);
  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  if ((PeriodicTimer1Handle == NULL) || (PeriodicTimer2Handle == NULL) || (OneShotTimer3Handle == NULL))
  {
    error_count++;
    PRINTF("Failed to initialize software timers\r\n");
  }
  else
  {
    /* Start software timers */
    xTimerStart(PeriodicTimer1Handle, 0);
    xTimerStart(PeriodicTimer2Handle, 0);
    xTimerStart(OneShotTimer3Handle, 0);
  }
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_THREADS */
  blinkLed1Handle = osThreadNew(blinkTask, NULL, &blinkLed1_attributes);
  if (blinkLed1Handle == NULL)
  {
    PRINTF("Failed to create user tasks\r\n");
    error_count++;
  }
  /* USER CODE END RTOS_THREADS */

  if (error_count == 0)
  {
    PRINT_VAR(error_count);
    PRINTF("Start RTOS_Kernel\r\n");
    osKernelStart();
  }
  else
  {
    PRINTF("Failed to start RTOS_Kernel\r\n");
  }
  /* Start scheduler */

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
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
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
     */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12,
                    GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA5 PA11 PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/* USER CODE BEGIN Header_blinkTask */
/**
 * @brief Function implementing the blinkLed1 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_blinkTask */
void blinkTask(void *argument)
{
  /* USER CODE BEGIN blinkTask */
  /* Infinite loop */
  for (;;)
  {
    toggleLed1;
    osDelay(1000);
  }
  /* USER CODE END blinkTask */
}

/* timer1Callback function */
void timer1Callback(void *argument)
{
  TickType_t tick_now;
  static uint32_t timer1_freq = TIMER_1_FREQ;
  /* Obtain current tick count */
  tick_now = xTaskGetTickCount();

  toggleLed4;
  newline;
  PRINTF("Timer1 executes every ");
  PRINT_VAR(timer1_freq);
  PRINT_VAR(tick_now);
}

/* timer2Callback function */
void timer2Callback(void *argument)
{
  TickType_t tick_now;
  static uint32_t timer2_freq = TIMER_2_FREQ;
  /* Count number of time Timer2 has expired */
  static uint32_t timer2_expired_count = 0;

  tick_now = xTaskGetTickCount();

  toggleLed2;
  newline;
  PRINTF("Timer2 execute every ");
  PRINT_VAR(timer2_freq);
  PRINT_VAR(tick_now);

  ++timer2_expired_count;
  /* Modify timer 2 period every 5 times expires */
  if (timer2_expired_count % 5 == 0)
  {
    PRINTF("Change timer 2 freq & Reset Timer 3\r\n");
    timer2_freq += 50;
    /* Modify timer 2 period */
    xTimerChangePeriod(PeriodicTimer2Handle, timer2_freq, 0);
    xTimerReset(OneShotTimer3Handle, 0);
    /* Print debug value */
  }
}
/* timer3Callback function */
void timer3Callback(void *argument)
{
  TickType_t tick_now;
  tick_now = xTaskGetTickCount(); /* Get current tick count */

  toggleLed3;
  newline;
  PRINTF("Timer3 execute after first 3333ms\r\n");
  PRINT_VAR(tick_now);
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
