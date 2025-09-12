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

/* ========================= Pin Map (PA4..PA15) ============================= */
/* USER CODE BEGIN PD */
/* ⚠️ PA13/PA14 are SWD (debug). Using them as GPIO disables debugging. */
#define c1_GPIO_Port   GPIOA
#define c1_Pin         GPIO_PIN_4
#define c2_GPIO_Port   GPIOA
#define c2_Pin         GPIO_PIN_5
#define c3_GPIO_Port   GPIOA
#define c3_Pin         GPIO_PIN_6
#define c4_GPIO_Port   GPIOA
#define c4_Pin         GPIO_PIN_7
#define c5_GPIO_Port   GPIOA
#define c5_Pin         GPIO_PIN_8
#define c6_GPIO_Port   GPIOA
#define c6_Pin         GPIO_PIN_9
#define c7_GPIO_Port   GPIOA
#define c7_Pin         GPIO_PIN_10
#define c8_GPIO_Port   GPIOA
#define c8_Pin         GPIO_PIN_11
#define c9_GPIO_Port   GPIOA
#define c9_Pin         GPIO_PIN_12
#define c10_GPIO_Port  GPIOA
#define c10_Pin        GPIO_PIN_13   // SWDIO (warning)
#define c11_GPIO_Port  GPIOA
#define c11_Pin        GPIO_PIN_14   // SWCLK (warning)
#define c12_GPIO_Port  GPIOA
#define c12_Pin        GPIO_PIN_15
/* USER CODE END PD */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

/* USER CODE BEGIN PFP */
static void clearAllClock(void);
static void setNumberOnClock(int num);
static void clearNumberOnClock(int num);
/* USER CODE END PFP */

/* ============================== Helpers ==================================== */
/* USER CODE BEGIN 0 */

/* Turn OFF all 12 LEDs (active-low → drive HIGH) */
static void clearAllClock(void)
{
  HAL_GPIO_WritePin(c1_GPIO_Port,  c1_Pin,  GPIO_PIN_SET);
  HAL_GPIO_WritePin(c2_GPIO_Port,  c2_Pin,  GPIO_PIN_SET);
  HAL_GPIO_WritePin(c3_GPIO_Port,  c3_Pin,  GPIO_PIN_SET);
  HAL_GPIO_WritePin(c4_GPIO_Port,  c4_Pin,  GPIO_PIN_SET);
  HAL_GPIO_WritePin(c5_GPIO_Port,  c5_Pin,  GPIO_PIN_SET);
  HAL_GPIO_WritePin(c6_GPIO_Port,  c6_Pin,  GPIO_PIN_SET);
  HAL_GPIO_WritePin(c7_GPIO_Port,  c7_Pin,  GPIO_PIN_SET);
  HAL_GPIO_WritePin(c8_GPIO_Port,  c8_Pin,  GPIO_PIN_SET);
  HAL_GPIO_WritePin(c9_GPIO_Port,  c9_Pin,  GPIO_PIN_SET);
  HAL_GPIO_WritePin(c10_GPIO_Port, c10_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(c11_GPIO_Port, c11_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(c12_GPIO_Port, c12_Pin, GPIO_PIN_SET);
}

/* Turn ON a specific LED (active-low) without clearing others.
   Input: 0..11 (0 represents "12 o'clock" on LED c12) */
static void setNumberOnClock(int num)
{
  int sel = num % 12;
  if (sel < 0) sel += 12;

  switch (sel)
  {
    case 1:  HAL_GPIO_WritePin(c1_GPIO_Port,  c1_Pin,  GPIO_PIN_RESET); break;
    case 2:  HAL_GPIO_WritePin(c2_GPIO_Port,  c2_Pin,  GPIO_PIN_RESET); break;
    case 3:  HAL_GPIO_WritePin(c3_GPIO_Port,  c3_Pin,  GPIO_PIN_RESET); break;
    case 4:  HAL_GPIO_WritePin(c4_GPIO_Port,  c4_Pin,  GPIO_PIN_RESET); break;
    case 5:  HAL_GPIO_WritePin(c5_GPIO_Port,  c5_Pin,  GPIO_PIN_RESET); break;
    case 6:  HAL_GPIO_WritePin(c6_GPIO_Port,  c6_Pin,  GPIO_PIN_RESET); break;
    case 7:  HAL_GPIO_WritePin(c7_GPIO_Port,  c7_Pin,  GPIO_PIN_RESET); break;
    case 8:  HAL_GPIO_WritePin(c8_GPIO_Port,  c8_Pin,  GPIO_PIN_RESET); break;
    case 9:  HAL_GPIO_WritePin(c9_GPIO_Port,  c9_Pin,  GPIO_PIN_RESET); break;
    case 10: HAL_GPIO_WritePin(c10_GPIO_Port, c10_Pin, GPIO_PIN_RESET); break;
    case 11: HAL_GPIO_WritePin(c11_GPIO_Port, c11_Pin, GPIO_PIN_RESET); break;
    case 0:  HAL_GPIO_WritePin(c12_GPIO_Port, c12_Pin, GPIO_PIN_RESET); break; // 12 o'clock
    default: break;
  }
}

/* Turn OFF a specific LED (active-low → drive HIGH) */
static void clearNumberOnClock(int num)
{
  int sel = num % 12;
  if (sel < 0) sel += 12;

  switch (sel)
  {
    case 1:  HAL_GPIO_WritePin(c1_GPIO_Port,  c1_Pin,  GPIO_PIN_SET); break;
    case 2:  HAL_GPIO_WritePin(c2_GPIO_Port,  c2_Pin,  GPIO_PIN_SET); break;
    case 3:  HAL_GPIO_WritePin(c3_GPIO_Port,  c3_Pin,  GPIO_PIN_SET); break;
    case 4:  HAL_GPIO_WritePin(c4_GPIO_Port,  c4_Pin,  GPIO_PIN_SET); break;
    case 5:  HAL_GPIO_WritePin(c5_GPIO_Port,  c5_Pin,  GPIO_PIN_SET); break;
    case 6:  HAL_GPIO_WritePin(c6_GPIO_Port,  c6_Pin,  GPIO_PIN_SET); break;
    case 7:  HAL_GPIO_WritePin(c7_GPIO_Port,  c7_Pin,  GPIO_PIN_SET); break;
    case 8:  HAL_GPIO_WritePin(c8_GPIO_Port,  c8_Pin,  GPIO_PIN_SET); break;
    case 9:  HAL_GPIO_WritePin(c9_GPIO_Port,  c9_Pin,  GPIO_PIN_SET); break;
    case 10: HAL_GPIO_WritePin(c10_GPIO_Port, c10_Pin, GPIO_PIN_SET); break;
    case 11: HAL_GPIO_WritePin(c11_GPIO_Port, c11_Pin, GPIO_PIN_SET); break;
    case 0:  HAL_GPIO_WritePin(c12_GPIO_Port, c12_Pin, GPIO_PIN_SET); break;   // 12 o'clock
    default: break;
  }
}
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

  /* Ensure all LEDs are OFF initially (active-low → SET) */
  clearAllClock();

  /* ========================== Clock state =========================== */
  int s = 0; // seconds (0..59)
  int m = 0; // minutes (0..59)
  int h = 0; // hours   (0..11)

  /* ========================== Main loop ============================= */
  while (1)
  {
    /* Reset previous display */
    clearAllClock();

    /* Advance time */
    s++;
    if (s >= 60) { s = 0; m++; }
    if (m >= 60) { m = 0; h++; }
    if (h >= 12) { h = 0; }

    /* Display three hands:
       - second hand at s/5 (0..11)
       - minute hand at m/5 (0..11)
       - hour hand at h     (0..11)
       Multiple hands can coincide on the same LED (that pin remains LOW). */
    setNumberOnClock(s / 5);
    setNumberOnClock(m / 5);
    setNumberOnClock(h);

    /* can lower the delay to progress faster. */
    HAL_Delay(1000);
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

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

  /* Initial output level: all OFF (active-low → SET) */
  HAL_GPIO_WritePin(GPIOA,
                    GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 |
                    GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 |
                    GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15,
                    GPIO_PIN_SET);

  /* Configure PA4..PA15 as outputs */
  GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 |
                        GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 |
                        GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* If you need SWD for debugging, avoid PA13/PA14 and remap those LEDs. */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1) { }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
  (void)file; (void)line;
}
#endif
