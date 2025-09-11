/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* ------- 7-seg & traffic LED pin mapping (adjust to your wiring) ------- */
/* 7-seg for road 1 (a..g) -> GPIOA */
#define a_GPIO_Port   GPIOA
#define a_Pin         GPIO_PIN_0
#define b_GPIO_Port   GPIOA
#define b_Pin         GPIO_PIN_1
#define c_GPIO_Port   GPIOA
#define c_Pin         GPIO_PIN_2
#define d_GPIO_Port   GPIOA
#define d_Pin         GPIO_PIN_3
#define e_GPIO_Port   GPIOA
#define e_Pin         GPIO_PIN_4
#define f_GPIO_Port   GPIOA
#define f_Pin         GPIO_PIN_5
#define g_GPIO_Port   GPIOA
#define g_Pin         GPIO_PIN_6

/* 7-seg for road 2 (a1..g1) -> GPIOB */
#define a1_GPIO_Port  GPIOB
#define a1_Pin        GPIO_PIN_0
#define b1_GPIO_Port  GPIOB
#define b1_Pin        GPIO_PIN_1
#define c1_GPIO_Port  GPIOB
#define c1_Pin        GPIO_PIN_2
#define d1_GPIO_Port  GPIOB
#define d1_Pin        GPIO_PIN_3
#define e1_GPIO_Port  GPIOB
#define e1_Pin        GPIO_PIN_4
#define f1_GPIO_Port  GPIOB
#define f1_Pin        GPIO_PIN_5
#define g1_GPIO_Port  GPIOB
#define g1_Pin        GPIO_PIN_6

/* Traffic LEDs for road 1 -> GPIOB */
#define LED_RED_GPIO_Port     GPIOB
#define LED_RED_Pin           GPIO_PIN_7
#define LED_YELLOW_GPIO_Port  GPIOB
#define LED_YELLOW_Pin        GPIO_PIN_8
#define LED_GREEN_GPIO_Port   GPIOB
#define LED_GREEN_Pin         GPIO_PIN_9

/* Traffic LEDs for road 2 -> GPIOB */
#define LED_RED1_GPIO_Port     GPIOB
#define LED_RED1_Pin           GPIO_PIN_10
#define LED_YELLOW1_GPIO_Port  GPIOB
#define LED_YELLOW1_Pin        GPIO_PIN_11
#define LED_GREEN1_GPIO_Port   GPIOB
#define LED_GREEN1_Pin         GPIO_PIN_12

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile int counter  = 0;
volatile int counter1 = 0;
volatile int dem  = 4; /* 7-seg countdown for road 1, begin at red */
volatile int dem1 = 2; /* 7-seg countdown for road 2, begin at green */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
void display7SEG (int num);
void display7SEG1(int num);
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
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

  /* USER CODE BEGIN 2 */
  /* Initial light states (common-anode style: SET = off, RESET = on) */
  HAL_GPIO_WritePin(LED_RED_GPIO_Port,     LED_RED_Pin,     GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port,  LED_YELLOW_Pin,  GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,   LED_GREEN_Pin,   GPIO_PIN_SET);

  HAL_GPIO_WritePin(LED_RED1_GPIO_Port,    LED_RED1_Pin,    GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED_YELLOW1_GPIO_Port, LED_YELLOW1_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port,  LED_GREEN1_Pin,  GPIO_PIN_SET);
  /* USER CODE END 2 */

  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* -------- 1st road lights -------- */
    if (counter < 5) {
      HAL_GPIO_WritePin(LED_RED_GPIO_Port,    LED_RED_Pin,    GPIO_PIN_RESET);
      HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_SET);
      if (dem < 0) dem = 4;
    }
    else if (counter < 8) {
      HAL_GPIO_WritePin(LED_RED_GPIO_Port,   LED_RED_Pin,   GPIO_PIN_SET);
      HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
      if (dem < 0) dem = 2;
    }
    else if (counter < 10) {
      HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,  LED_GREEN_Pin,  GPIO_PIN_SET);
      if (dem < 0) dem = 1;
    }

    /* -------- 2nd road lights -------- */
    if (counter1 < 3) {
      HAL_GPIO_WritePin(LED_RED1_GPIO_Port,   LED_RED1_Pin,   GPIO_PIN_SET);
      HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, GPIO_PIN_RESET);
      if (dem1 < 0) dem1 = 2;
    }
    else if (counter1 < 5) {
      HAL_GPIO_WritePin(LED_YELLOW1_GPIO_Port, LED_YELLOW1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port,  LED_GREEN1_Pin,  GPIO_PIN_SET);
      if (dem1 < 0) dem1 = 1;
    }
    else if (counter1 < 10) {
      HAL_GPIO_WritePin(LED_RED1_GPIO_Port,    LED_RED1_Pin,    GPIO_PIN_RESET);
      HAL_GPIO_WritePin(LED_YELLOW1_GPIO_Port, LED_YELLOW1_Pin, GPIO_PIN_SET);
      if (dem1 < 0) dem1 = 4;
    }

    /* -------- Update 7-seg displays -------- */
    display7SEG (dem--);   /* 1st road */
    display7SEG1(dem1--);  /* 2nd road */

    /* -------- Tick counters -------- */
    counter  = (counter  + 1) % 10;
    counter1 = (counter1 + 1) % 10;

    HAL_Delay(1000);
  }
  /* USER CODE END WHILE */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /* Initializes the RCC Oscillators */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Initializes the CPU, AHB and APB buses clocks */
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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* Default output levels */
  HAL_GPIO_WritePin(GPIOA, a_Pin|b_Pin|c_Pin|d_Pin|e_Pin|f_Pin|g_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, a1_Pin|b1_Pin|c1_Pin|d1_Pin|e1_Pin|f1_Pin|g1_Pin|
                           LED_RED_Pin|LED_YELLOW_Pin|LED_GREEN_Pin|
                           LED_RED1_Pin|LED_YELLOW1_Pin|LED_GREEN1_Pin, GPIO_PIN_SET);

  /* GPIOA: 7-seg road 1 */
  GPIO_InitStruct.Pin = a_Pin|b_Pin|c_Pin|d_Pin|e_Pin|f_Pin|g_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* GPIOB: 7-seg road 2 + traffic LEDs */
  GPIO_InitStruct.Pin = a1_Pin|b1_Pin|c1_Pin|d1_Pin|e1_Pin|f1_Pin|g1_Pin|
                        LED_RED_Pin|LED_YELLOW_Pin|LED_GREEN_Pin|
                        LED_RED1_Pin|LED_YELLOW1_Pin|LED_GREEN1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */
void display7SEG (int num)
{
  switch (num) {
    case 0:
      HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, GPIO_PIN_SET);
      break;
    case 1:
      HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, GPIO_PIN_SET);
      break;
    case 2:
      HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, GPIO_PIN_RESET);
      break;
    case 3:
      HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, GPIO_PIN_RESET);
      break;
    case 4:
      HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, GPIO_PIN_RESET);
      break;
    case 5:
      HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, GPIO_PIN_RESET);
      break;
    case 6:
      HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, GPIO_PIN_RESET);
      break;
    case 7:
      HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, GPIO_PIN_SET);
      break;
    case 8:
      HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, GPIO_PIN_RESET);
      break;
    case 9:
      HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, GPIO_PIN_RESET);
      break;
    default:
      HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, GPIO_PIN_SET);
      break;
  }
}

void display7SEG1 (int num)
{
  switch (num) {
    case 0:
      HAL_GPIO_WritePin(a1_GPIO_Port, a1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(b1_GPIO_Port, b1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(c1_GPIO_Port, c1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(d1_GPIO_Port, d1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(e1_GPIO_Port, e1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(f1_GPIO_Port, f1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(g1_GPIO_Port, g1_Pin, GPIO_PIN_SET);
      break;
    case 1:
      HAL_GPIO_WritePin(a1_GPIO_Port, a1_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(b1_GPIO_Port, b1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(c1_GPIO_Port, c1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(d1_GPIO_Port, d1_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(e1_GPIO_Port, e1_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(f1_GPIO_Port, f1_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(g1_GPIO_Port, g1_Pin, GPIO_PIN_SET);
      break;
    case 2:
      HAL_GPIO_WritePin(a1_GPIO_Port, a1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(b1_GPIO_Port, b1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(c1_GPIO_Port, c1_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(d1_GPIO_Port, d1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(e1_GPIO_Port, e1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(f1_GPIO_Port, f1_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(g1_GPIO_Port, g1_Pin, GPIO_PIN_RESET);
      break;
    case 3:
      HAL_GPIO_WritePin(a1_GPIO_Port, a1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(b1_GPIO_Port, b1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(c1_GPIO_Port, c1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(d1_GPIO_Port, d1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(e1_GPIO_Port, e1_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(f1_GPIO_Port, f1_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(g1_GPIO_Port, g1_Pin, GPIO_PIN_RESET);
      break;
    case 4:
      HAL_GPIO_WritePin(a1_GPIO_Port, a1_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(b1_GPIO_Port, b1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(c1_GPIO_Port, c1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(d1_GPIO_Port, d1_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(e1_GPIO_Port, e1_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(f1_GPIO_Port, f1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(g1_GPIO_Port, g1_Pin, GPIO_PIN_RESET);
      break;
    case 5:
      HAL_GPIO_WritePin(a1_GPIO_Port, a1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(b1_GPIO_Port, b1_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(c1_GPIO_Port, c1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(d1_GPIO_Port, d1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(e1_GPIO_Port, e1_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(f1_GPIO_Port, f1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(g1_GPIO_Port, g1_Pin, GPIO_PIN_RESET);
      break;
    case 6:
      HAL_GPIO_WritePin(a1_GPIO_Port, a1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(b1_GPIO_Port, b1_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(c1_GPIO_Port, c1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(d1_GPIO_Port, d1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(e1_GPIO_Port, e1_Pin, GPIO_PIN_RESET); /* <-- fixed */
      HAL_GPIO_WritePin(f1_GPIO_Port, f1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(g1_GPIO_Port, g1_Pin, GPIO_PIN_RESET);
      break;
    case 7:
      HAL_GPIO_WritePin(a1_GPIO_Port, a1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(b1_GPIO_Port, b1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(c1_GPIO_Port, c1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(d1_GPIO_Port, d1_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(e1_GPIO_Port, e1_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(f1_GPIO_Port, f1_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(g1_GPIO_Port, g1_Pin, GPIO_PIN_SET);
      break;
    case 8:
      HAL_GPIO_WritePin(a1_GPIO_Port, a1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(b1_GPIO_Port, b1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(c1_GPIO_Port, c1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(d1_GPIO_Port, d1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(e1_GPIO_Port, e1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(f1_GPIO_Port, f1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(g1_GPIO_Port, g1_Pin, GPIO_PIN_RESET);
      break;
    case 9:
      HAL_GPIO_WritePin(a1_GPIO_Port, a1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(b1_GPIO_Port, b1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(c1_GPIO_Port, c1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(d1_GPIO_Port, d1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(e1_GPIO_Port, e1_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(f1_GPIO_Port, f1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(g1_GPIO_Port, g1_Pin, GPIO_PIN_RESET);
      break;
    default:
      HAL_GPIO_WritePin(a1_GPIO_Port, a1_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(b1_GPIO_Port, b1_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(c1_GPIO_Port, c1_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(d1_GPIO_Port, d1_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(e1_GPIO_Port, e1_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(f1_GPIO_Port, f1_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(g1_GPIO_Port, g1_Pin, GPIO_PIN_SET);
      break;
  }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1) {}
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
  /* Optionally report file and line */
}
#endif /* USE_FULL_ASSERT */
