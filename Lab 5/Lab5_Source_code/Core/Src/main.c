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

// FSM cho việc đọc lệnh từ buffer
typedef enum {
    CMD_IDLE,       // Chờ ký tự '!'
    CMD_RECEIVING   // Đang nhận data giữa '!' và '#'
} CommandParserState;
CommandParserState cmd_parser_state = CMD_IDLE;

// FSM cho giao thức UART
typedef enum {
    UART_IDLE,        // Trạng thái nghỉ, chờ lệnh !RST#
    UART_WAIT_ACK     // Đã gửi !ADC=...#, đang chờ !OK#
} UartCommunicationState;
UartCommunicationState uart_state = UART_IDLE;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MAX_BUFFER_SIZE 30
#define MAX_CMD_LENGTH 10
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
// Biến cho UART
uint8_t temp = 0;
uint8_t buffer[MAX_BUFFER_SIZE];
uint8_t index_buffer = 0;
uint8_t buffer_flag = 0;

// Biến cho Command Parser
uint8_t cmd_data[MAX_CMD_LENGTH];
uint8_t cmd_flag = 0;
static uint8_t cmd_index = 0;

// Biến cho FSM
uint32_t ADC_value = 0;
uint8_t str[20];
uint8_t ack_received = 0;
uint8_t resend_flag = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
void command_parser_fsm(void);
void uart_communication_fsm(void);
void read_ADC(void);
void send_ADC_packet(void);
void timeout_task(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
  * @brief Đọc giá trị ADC
  */
void read_ADC(void) {
	  HAL_ADC_Start(&hadc1);
	  HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	  ADC_value = HAL_ADC_GetValue(&hadc1);
	  HAL_ADC_Stop(&hadc1);
}

/**
  * @brief Gửi gói tin ADC qua UART (NON-BLOCKING)
  */
void send_ADC_packet(void) {
	  // Giá trị ADC_value đã được đọc từ trước
	  HAL_UART_Transmit(&huart2, (uint8_t*)str, sprintf(str, "!ADC=%ld#", ADC_value), 1000);
}

/**
  * @brief Task này được scheduler gọi sau 3 giây
  */
void timeout_task(void) {
	// Nếu sau 3 giây mà vẫn chưa nhận được ACK
	if (ack_received == 0) {
		// Đặt cờ resend để FSM trong while(1) biết và gửi lại
		resend_flag = 1;
	}
}

/**
  * @brief FSM phân tích lệnh từ buffer
  */
void command_parser_fsm(void) {
	// Lấy ký tự mới nhất từ buffer ngắt
	uint8_t received_char = buffer[index_buffer - 1];

	switch (cmd_parser_state){
		case CMD_IDLE:
			// Chờ ký tự bắt đầu
			if (received_char == '!') {
				cmd_parser_state = CMD_RECEIVING;
				cmd_index = 0; // Reset chỉ số data
			}
			break;

		case CMD_RECEIVING:
			if (received_char == '#'){
				// Kết thúc lệnh
				cmd_data[cmd_index] = '\0'; // Thêm ký tự kết thúc chuỗi
				cmd_parser_state = CMD_IDLE; // Quay về chờ lệnh mới
				cmd_flag = 1; // Đặt cờ báo có lệnh mới cho FSM chính
			}
			else if (cmd_index < MAX_CMD_LENGTH - 1){
				// Lưu data vào
				cmd_data[cmd_index++] = received_char;
			} else {
				// Lỗi, tràn bộ đệm lệnh -> Reset
				cmd_parser_state = CMD_IDLE;
				cmd_index = 0;
			}
			break;

		default:
			cmd_parser_state = CMD_IDLE;
			break;
	}
}

/**
  * @brief FSM quản lý giao thức UART (Flow & Error Control)
  */
void uart_communication_fsm(void){
    switch (uart_state) {
        case UART_IDLE:
            // Chỉ hoạt động khi có lệnh !RST#
            if (cmd_flag == 1) {
            	cmd_flag = 0; // Xóa cờ
                if (strcmp((char *)cmd_data, "RST") == 0) {
                    // 1. Đọc ADC
                	read_ADC();
                	// 2. Gửi gói tin !ADC=...#
                    send_ADC_packet();
                    // 3. Reset cờ
                    ack_received = 0;
                    resend_flag = 0;
                    // 4. Thêm task timeout 3 giây (3000ms)
                    SCH_Add_Task(timeout_task, 3000, 0); // 0 = one-shot task
                    // 5. Chuyển sang trạng thái chờ ACK
                    uart_state = UART_WAIT_ACK;

                    HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin); // Nháy đèn báo đã gửi
                }
            }
            break;

        case UART_WAIT_ACK:
            // TRƯỜNG HỢP 1: Nhận được lệnh mới trong khi chờ
            if (cmd_flag == 1) {
            	cmd_flag = 0; // Xóa cờ
                if (strcmp((char *)cmd_data, "OK") == 0) {
                    // ĐÃ NHẬN ĐƯỢC ACK (!OK#)
                    ack_received = 1; // Đặt cờ
                    uart_state = UART_IDLE; // Quay về trạng thái nghỉ
                    HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin); // Nháy đèn báo đã nhận OK
                }
                else if (strcmp((char *)cmd_data, "RST") == 0) {
					// User gửi lại !RST# -> Xử lý như một lệnh mới
					// (Hủy task cũ, làm lại từ đầu)
					// (Vì chưa có hàm SCH_Delete_Task nên ta bỏ qua, chỉ cần chạy lại)
					read_ADC();
                    send_ADC_packet();
                    ack_received = 0;
                    resend_flag = 0;
                    SCH_Add_Task(timeout_task, 3000, 0); // Thêm task timeout 3 giây MỚI
                    // Vẫn ở trạng thái UART_WAIT_ACK
				}
            }

            // TRƯỜNG HỢP 2: Hết 3 giây mà chưa nhận được ACK
            if (resend_flag == 1) {
            	resend_flag = 0; // Xóa cờ

            	// 1. Gửi lại gói tin !ADC=...# (với giá trị ADC cũ)
            	send_ADC_packet();
            	// 2. Thêm lại task timeout 3 giây
            	SCH_Add_Task(timeout_task, 3000, 0);
            	// 3. Vẫn ở trạng thái chờ UART_WAIT_ACK
            }
            break;

        default:
        	uart_state = UART_IDLE;
            break;
    }
}

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

  // BẮT ĐẦU nhận 1 byte bằng interrupt
  HAL_UART_Receive_IT(&huart2, &temp, 1);

  // BẮT ĐẦU timer 10ms
  HAL_TIM_Base_Start_IT(&htim2);

  // KHỞI TẠO scheduler
  SCH_Init();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

	  // 1. Chạy các task đã đến hạn (như task timeout 3s)
	  SCH_Dispatch_Tasks();

	  // 2. Nếu có dữ liệu mới từ UART, chạy FSM phân tích lệnh
	  if (buffer_flag == 1) {
		  command_parser_fsm();
		  buffer_flag = 0; // Xóa cờ
	  }

	  // 3. Chạy FSM logic chính
	  uart_communication_fsm();

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

  /* USER CODE END 2 */

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
    	// Thêm ký tự vào buffer
        buffer[index_buffer++] = temp;

        // Reset buffer nếu bị đầy
        if (index_buffer == MAX_BUFFER_SIZE) {
            index_buffer = 0;
        }

        // Đặt cờ báo cho vòng lặp while(1) biết có data mới
        buffer_flag = 1;

        // Kích hoạt lại ngắt để nhận ký tự tiếp theo
        HAL_UART_Receive_IT(&huart2, &temp, 1);
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim)
{
    if(htim->Instance == TIM2){
    	SCH_Update(); // "Tích" nhịp cho scheduler mỗi 10ms
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
  * where the assert_param error has occurred.
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
