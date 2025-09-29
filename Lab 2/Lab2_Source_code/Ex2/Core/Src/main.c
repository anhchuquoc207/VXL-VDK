#include "main.h"

#define LED1_GPIO_Port   GPIOA
#define LED1_Pin         GPIO_PIN_5
#define LED2_GPIO_Port   GPIOA
#define LED2_Pin         GPIO_PIN_4

#define EN0_GPIO_Port    GPIOA
#define EN0_Pin          GPIO_PIN_6
#define EN1_GPIO_Port    GPIOA
#define EN1_Pin          GPIO_PIN_7
#define EN2_GPIO_Port    GPIOA
#define EN2_Pin          GPIO_PIN_8
#define EN3_GPIO_Port    GPIOA
#define EN3_Pin          GPIO_PIN_9

#define SEG_A_GPIO_Port  GPIOB
#define SEG_A_Pin        GPIO_PIN_0
#define SEG_B_GPIO_Port  GPIOB
#define SEG_B_Pin        GPIO_PIN_1
#define SEG_C_GPIO_Port  GPIOB
#define SEG_C_Pin        GPIO_PIN_2
#define SEG_D_GPIO_Port  GPIOB
#define SEG_D_Pin        GPIO_PIN_3
#define SEG_E_GPIO_Port  GPIOB
#define SEG_E_Pin        GPIO_PIN_4
#define SEG_F_GPIO_Port  GPIOB
#define SEG_F_Pin        GPIO_PIN_5
#define SEG_G_GPIO_Port  GPIOB
#define SEG_G_Pin        GPIO_PIN_6

#define SEG_ON   GPIO_PIN_RESET
#define SEG_OFF  GPIO_PIN_SET

#define TICKS_500MS 50
#define TICKS_1S    100

TIM_HandleTypeDef htim2;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);

static void display7SEG(int num)
{
  static const uint8_t map[10] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
  };
  if (num < 0 || num > 9) num = 0;
  uint8_t m = map[num];

  HAL_GPIO_WritePin(SEG_A_GPIO_Port, SEG_A_Pin, (m & 0x01)? SEG_ON : SEG_OFF);
  HAL_GPIO_WritePin(SEG_B_GPIO_Port, SEG_B_Pin, (m & 0x02)? SEG_ON : SEG_OFF);
  HAL_GPIO_WritePin(SEG_C_GPIO_Port, SEG_C_Pin, (m & 0x04)? SEG_ON : SEG_OFF);
  HAL_GPIO_WritePin(SEG_D_GPIO_Port, SEG_D_Pin, (m & 0x08)? SEG_ON : SEG_OFF);
  HAL_GPIO_WritePin(SEG_E_GPIO_Port, SEG_E_Pin, (m & 0x10)? SEG_ON : SEG_OFF);
  HAL_GPIO_WritePin(SEG_F_GPIO_Port, SEG_F_Pin, (m & 0x20)? SEG_ON : SEG_OFF);
  HAL_GPIO_WritePin(SEG_G_GPIO_Port, SEG_G_Pin, (m & 0x40)? SEG_ON : SEG_OFF);
}

static void select_digit(uint8_t idx)
{
  HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_SET);

  switch (idx) {
    case 0: HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, GPIO_PIN_RESET); break;
    case 1: HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_RESET); break;
    case 2: HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_RESET); break;
    case 3: HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_RESET); break;
    default: break;
  }
}

static void toggle_leds(void)
{
  HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
  HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
}

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_TIM2_Init();

  HAL_TIM_Base_Start_IT(&htim2);

  while (1) {
  }
}

static int tick500 = TICKS_500MS;
static int tick1s  = TICKS_1S;

static const uint8_t digit_order[4] = { 0, 1, 2, 3 };
static const uint8_t digit_value[4] = { 1, 2, 3, 0 };
static uint8_t idx = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance != TIM2) return;

  if (--tick500 <= 0) {
    tick500 = TICKS_500MS;
    idx = (uint8_t)((idx + 1) & 0x03);
  }

  select_digit(digit_order[idx]);
  display7SEG(digit_value[idx]);

  if (--tick1s <= 0) {
    tick1s = TICKS_1S;
    toggle_leds();
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) { Error_Handler(); }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              | RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) { Error_Handler(); }
}

static void MX_TIM2_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK) { Error_Handler(); }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK) { Error_Handler(); }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK) { Error_Handler(); }
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOA, LED1_Pin|LED2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, EN0_Pin|EN1_Pin|EN2_Pin|EN3_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, SEG_A_Pin|SEG_B_Pin|SEG_C_Pin|SEG_D_Pin
                          |SEG_E_Pin|SEG_F_Pin|SEG_G_Pin, GPIO_PIN_SET);

  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  GPIO_InitStruct.Pin = LED1_Pin|LED2_Pin|EN0_Pin|EN1_Pin|EN2_Pin|EN3_Pin;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = SEG_A_Pin|SEG_B_Pin|SEG_C_Pin|SEG_D_Pin
                       |SEG_E_Pin|SEG_F_Pin|SEG_G_Pin;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void Error_Handler(void)
{
  __disable_irq();
  while (1) { }
}
