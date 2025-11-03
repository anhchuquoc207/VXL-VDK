/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "sched.h"
#include <string.h>
#include <stdio.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
    IDLE,
    SOC,
    EOC
} CommandRead;
CommandRead current_read_state = IDLE;

static enum {
	AUTO,
	CMD_CHECK,
	SEND,
	ACK,
	FAILURE,
	RESEND
} current_state = AUTO;
//comm_state current_state = IDLE;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define MAX_BUFFER_SIZE 30
const char askSen[3] = "RST";
const char stopSen[2] = "OK";
uint8_t temp = 0;
uint8_t buffer[MAX_BUFFER_SIZE];
uint8_t index_buffer = 0;
uint8_t buffer_flag = 1;

#define MAX_CMD_LENGTH 10

uint8_t cmd_data[MAX_CMD_LENGTH];
uint8_t cmd_flag = 0;
static uint8_t cmd_index = 0;

#define MAX_RETRIES 3
uint8_t auto_action_flag = -1;
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
  MX_ADC1_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  //HAL_UART_Receive_IT(&huart2, &temp, 1);
  HAL_UART_RxCpltCallback(&huart2);
  HAL_TIM_Base_Start_IT(&htim2);
  SCH_Init();


  uint32_t ADC_value = 0;
  uint8_t str[20];

  void read_ADC(void) {
	  HAL_ADC_Start(&hadc1);
	  HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	  ADC_value = HAL_ADC_GetValue(&hadc1);
	  HAL_ADC_Stop(&hadc1);
  }

  void cmd_action(void){
	  //read_ADC();
	  HAL_UART_Transmit(&huart2, (char*)str, sprintf(str, "!ADC=%d#", ADC_value), 1000);
	  HAL_Delay(3000);
  }


  //SCH_Add_Task(blinkyLed, 1000, 100);


  void command_parser_fsm(void) {

    	  uint8_t received_char = buffer[index_buffer - 1];
    	  switch (current_read_state){
          	  case SOC:
          		  if (received_char == '!') {
          			  auto_action_flag = 0;
          			  current_read_state = EOC;
          			  cmd_index = 0;
          			  HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
          		  }
          		  break;
          	  case EOC:
          		  if (received_char == '#'){
          			  cmd_data[cmd_index] = '\0';
          			  //cmd_index--;
          			  current_read_state = SOC;
          			  cmd_flag = 1;
          			  //HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
          		  }
          		  else if (cmd_index < MAX_CMD_LENGTH - 1){
          			  cmd_data[cmd_index++] = received_char;
          		  }
          		  break;
          	  default:
          		  current_read_state = SOC;
          		  break;
    	  }
  }

  void uart_communiation_fsm(void){
      //static uint8_t retry_count = 0;
      switch (current_state) {
          case AUTO:
              if (cmd_flag == 1) {
            	  current_state = CMD_CHECK;
            	  //HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
                  cmd_flag = 0;
              }
              else if (auto_action_flag == 1){
            	  current_state = SEND;
              }
              break;

          case CMD_CHECK:
              if (strncmp((char *)cmd_data, stopSen, 2) == 0) {
            	  current_state = AUTO;
                  auto_action_flag = 0;
                  HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
              }
              else if (strncmp((char *)cmd_data, askSen, 3) == 0) {
            	  read_ADC();
            	  current_state = SEND;
            	  auto_action_flag = 1;
              }
              current_state = AUTO;
              break;

          case SEND:
        	  //SCH_Add_Task(cmd_action, 0, 3000);
              cmd_action();
              current_state = AUTO;
              break;

//          case ACK:
//              if (ack_flag == 1) {
//                  ack_flag = 0;
//                  comm_state = IDLE;
//              } else if (retry_count < MAX_RETRIES) {
//                  comm_state = RESEND;
//              } else {
//                  comm_state = FAILURE;
//              }
//              break;
//
//          case FAILURE:
//              retry_count = 0;
//              comm_state = IDLE;
//              break;
//
//          case RESEND:
//              retry_count++;
//              HAL_UART_Transmit(&huart2, cmd_data, strlen((char *)cmd_data), HAL_MAX_DELAY);
//              comm_state = ACK;
//              break;

          default:
        	  current_state = AUTO;
              break;
      }
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

	  if (buffer_flag == 1) {
		  auto_action_flag = 0;
		  command_parser_fsm();
		  //HAL_UART_Transmit(&huart2, (char*)cmd_data, sizeof(cmd_data), HAL_MAX_DELAY);
		  buffer_flag = 0;

	  }
	  uart_communiation_fsm();
    /* USER CODE BEGIN 3 */
//	  HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
//	  HAL_Delay(200);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

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
  huart2.Init.BaudRate = 9600;
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
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_RED_Pin */
  GPIO_InitStruct.Pin = LED_RED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_RED_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2) {
        buffer[index_buffer++] = temp;

        if (index_buffer == MAX_BUFFER_SIZE || cmd_flag == 1) {
            //buffer[index_buffer] = '\0';
            index_buffer = 0;
        }
//        if (temp == '\r'){
//        	uint8_t i = index_buffer;
//        	if (buffer[i - 5] == '!' &&
//        		buffer[i - 4] == 'R' &&
//				buffer[i - 3] == 'S' &&
//				buffer[i - 2] == 'T' &&
//				buffer[i - 1] == '#') buffer_flag = 1;
//        	else if (buffer[i - 4] == '!' &&
//        			buffer[i - 3] == 'O' &&
//					buffer[i - 2] == 'K' &&
//					buffer[i - 1] == '#') buffer_flag = 0;

//        char sub1[6];
//        for (int i = 0; i < MAX_BUFFER_SIZE; i++){
//        	if (buffer[i] == '!'){
//        		strncpy(sub1, (char *)buffer + i, 5);
//        		if (strcmp(sub1, askSen) == 0) {
//        			buffer_flag = 1;
//        			break;
//        		}
//        	}
//        }
//        memset(buffer, 0, sizeof(buffer));
//        }

        buffer_flag = 1;

        HAL_UART_Receive_IT(&huart2, &temp, 1); // Tiếp tục nhận ký tự
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim)
{
    if(htim->Instance == TIM2){
    	SCH_Update();
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
