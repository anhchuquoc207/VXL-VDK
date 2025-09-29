#include "main.h"

#define e0_Pin         GPIO_PIN_0
#define e0_GPIO_Port   GPIOA
#define e1_Pin         GPIO_PIN_1
#define e1_GPIO_Port   GPIOA

#define a_GPIO_Port    GPIOB
#define a_Pin          GPIO_PIN_0
#define b_GPIO_Port    GPIOB
#define b_Pin          GPIO_PIN_1
#define c_GPIO_Port    GPIOB
#define c_Pin          GPIO_PIN_2
#define d_GPIO_Port    GPIOB
#define d_Pin          GPIO_PIN_3
#define e_GPIO_Port    GPIOB
#define e_Pin          GPIO_PIN_4
#define f_GPIO_Port    GPIOB
#define f_Pin          GPIO_PIN_5
#define g_GPIO_Port    GPIOB
#define g_Pin          GPIO_PIN_6

// Active-low segments
#define SEG_ON   GPIO_PIN_RESET
#define SEG_OFF  GPIO_PIN_SET

TIM_HandleTypeDef htim2;

/* Prototypes */
void display7SEG (int num);
void SystemClock_Config(void);
static void MX_TIM2_Init(void);
static void MX_GPIO_Init(void);
static void led_red(void);
static void SW(int sw);

/* ====== Multiplex & swap state ====== */
/* Timer settings: prescaler 7999, period 9 -> update every 10 ms.
 * TIME_COUNTER = 50 -> 50 * 10 ms = 500 ms swap period.
 */
static const int TIME_COUNTER = 50;
static volatile int counter    = TIME_COUNTER;  // 500 ms swap counter
static volatile uint8_t mux    = 0;             // 0 -> first digit, 1 -> second digit
static volatile uint8_t d1     = 1;             // value for first digit
static volatile uint8_t d2     = 2;             // value for second digit

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_TIM2_Init();

  HAL_TIM_Base_Start_IT(&htim2);

  while (1) { }
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
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
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
  htim2.Init.Prescaler = 7999;                 // 8 MHz / (7999+1) = 1 kHz
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;                       // 10 ticks -> 100 Hz -> 10 ms
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

  // Red LED (optional)
  HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
  GPIO_InitStruct.Pin = LED_RED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_RED_GPIO_Port, &GPIO_InitStruct);

  // Digit enable pins (assumed active-low)
  HAL_GPIO_WritePin(e0_GPIO_Port, e0_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(e1_GPIO_Port, e1_Pin, GPIO_PIN_SET);
  GPIO_InitStruct.Pin = e0_Pin | e1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(e0_GPIO_Port, &GPIO_InitStruct);

  // Segment pins default OFF
  HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, SEG_OFF);
  HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, SEG_OFF);
  HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, SEG_OFF);
  HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, SEG_OFF);
  HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, SEG_OFF);
  HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, SEG_OFF);
  HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, SEG_OFF);

  GPIO_InitStruct.Pin = a_Pin|b_Pin|c_Pin|d_Pin|e_Pin|f_Pin|g_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(a_GPIO_Port, &GPIO_InitStruct);
}

static void led_red(void)
{
  HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
}

// Select digit: 1 -> first, 2 -> second, 0 -> both off
static void SW(int sw)
{
  switch (sw)
  {
    case 1: // enable first, disable second
      HAL_GPIO_WritePin(e0_GPIO_Port, e0_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(e1_GPIO_Port, e1_Pin, GPIO_PIN_SET);
      break;
    case 2: // disable first, enable second
      HAL_GPIO_WritePin(e0_GPIO_Port, e0_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(e1_GPIO_Port, e1_Pin, GPIO_PIN_RESET);
      break;
    default: // disable both
      HAL_GPIO_WritePin(e0_GPIO_Port, e0_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(e1_GPIO_Port, e1_Pin, GPIO_PIN_SET);
      break;
  }
}

void display7SEG (int num)
{
  // clear
  HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, SEG_OFF);
  HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, SEG_OFF);
  HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, SEG_OFF);
  HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, SEG_OFF);
  HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, SEG_OFF);
  HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, SEG_OFF);
  HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, SEG_OFF);

  switch (num)
  {
    case 0:
      HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, SEG_ON);
      HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, SEG_ON);
      HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, SEG_ON);
      HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, SEG_ON);
      HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, SEG_ON);
      HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, SEG_ON);
      break;

    case 1:
      HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, SEG_ON);
      HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, SEG_ON);
      break;

    case 2:
      HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, SEG_ON);
      HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, SEG_ON);
      HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, SEG_ON);
      HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, SEG_ON);
      HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, SEG_ON);
      break;

    default:
      break;
  }
}

/* ====== Timer ISR ======
 * - Every 10 ms: multiplex between digits.
 * - Every 500 ms: swap the numbers assigned to each digit.
 */
void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim)
{
  if (htim->Instance != TIM2) return;

  // 500 ms swap logic
  if (--counter <= 0)
  {
    counter = TIME_COUNTER; // 50 * 10 ms = 500 ms
    led_red();              // heartbeat on LED
    uint8_t t = d1; d1 = d2; d2 = t; // swap "1" and "2" between digits
  }

  // Fast multiplexing (every 10 ms here)
  SW(0); // disable both before changing segments

  if (mux == 0) {           // refresh first digit
    display7SEG(d1);
    SW(1);
  } else {                  // refresh second digit
    display7SEG(d2);
    SW(2);
  }
  mux ^= 1;                 // next tick, toggle digit
}

void Error_Handler(void)
{
  __disable_irq();
  while (1) { }
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
  (void)file; (void)line;
}
#endif /* USE_FULL_ASSERT */
