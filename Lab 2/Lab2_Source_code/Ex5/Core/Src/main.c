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

#define SEG0_GPIO_Port  GPIOB
#define SEG0_Pin        GPIO_PIN_0
#define SEG1_GPIO_Port  GPIOB
#define SEG1_Pin        GPIO_PIN_1
#define SEG2_GPIO_Port  GPIOB
#define SEG2_Pin        GPIO_PIN_2
#define SEG3_GPIO_Port  GPIOB
#define SEG3_Pin        GPIO_PIN_3
#define SEG4_GPIO_Port  GPIOB
#define SEG4_Pin        GPIO_PIN_4
#define SEG5_GPIO_Port  GPIOB
#define SEG5_Pin        GPIO_PIN_5
#define SEG6_GPIO_Port  GPIOB
#define SEG6_Pin        GPIO_PIN_6

#define SEG_ON   GPIO_PIN_RESET
#define SEG_OFF  GPIO_PIN_SET

TIM_HandleTypeDef htim2;

int hour   = 15;
int minute = 8;
int second = 50;

volatile int index_led = 0;
int led_buffer[4] = {0,0,0,0};

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);

static inline void EN_AllOff(void);
static inline void EN_Select(int idx);
void display7SEG(int num);
void update7SEG(int index);
void updateClockBuffer(void);
void led_red(void);

void display7SEG(int num){
    switch(num){
    case 0: HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG4_GPIO_Port, SEG4_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG5_GPIO_Port, SEG5_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG6_GPIO_Port, SEG6_Pin, SEG_OFF); break;
    case 1: HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, SEG_OFF);
            HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, SEG_OFF);
            HAL_GPIO_WritePin(SEG4_GPIO_Port, SEG4_Pin, SEG_OFF);
            HAL_GPIO_WritePin(SEG5_GPIO_Port, SEG5_Pin, SEG_OFF);
            HAL_GPIO_WritePin(SEG6_GPIO_Port, SEG6_Pin, SEG_OFF); break;
    case 2: HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, SEG_OFF);
            HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG4_GPIO_Port, SEG4_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG5_GPIO_Port, SEG5_Pin, SEG_OFF);
            HAL_GPIO_WritePin(SEG6_GPIO_Port, SEG6_Pin, SEG_ON); break;
    case 3: HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG4_GPIO_Port, SEG4_Pin, SEG_OFF);
            HAL_GPIO_WritePin(SEG5_GPIO_Port, SEG5_Pin, SEG_OFF);
            HAL_GPIO_WritePin(SEG6_GPIO_Port, SEG6_Pin, SEG_ON); break;
    case 4: HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, SEG_OFF);
            HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, SEG_OFF);
            HAL_GPIO_WritePin(SEG4_GPIO_Port, SEG4_Pin, SEG_OFF);
            HAL_GPIO_WritePin(SEG5_GPIO_Port, SEG5_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG6_GPIO_Port, SEG6_Pin, SEG_ON); break;
    case 5: HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, SEG_OFF);
            HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG4_GPIO_Port, SEG4_Pin, SEG_OFF);
            HAL_GPIO_WritePin(SEG5_GPIO_Port, SEG5_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG6_GPIO_Port, SEG6_Pin, SEG_ON); break;
    case 6: HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, SEG_OFF);
            HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG4_GPIO_Port, SEG4_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG5_GPIO_Port, SEG5_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG6_GPIO_Port, SEG6_Pin, SEG_ON); break;
    case 7: HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, SEG_OFF);
            HAL_GPIO_WritePin(SEG4_GPIO_Port, SEG4_Pin, SEG_OFF);
            HAL_GPIO_WritePin(SEG5_GPIO_Port, SEG5_Pin, SEG_OFF);
            HAL_GPIO_WritePin(SEG6_GPIO_Port, SEG6_Pin, SEG_OFF); break;
    case 8: HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG4_GPIO_Port, SEG4_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG5_GPIO_Port, SEG5_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG6_GPIO_Port, SEG6_Pin, SEG_ON); break;
    case 9: HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG4_GPIO_Port, SEG4_Pin, SEG_OFF);
            HAL_GPIO_WritePin(SEG5_GPIO_Port, SEG5_Pin, SEG_ON);
            HAL_GPIO_WritePin(SEG6_GPIO_Port, SEG6_Pin, SEG_ON); break;
    default:
            HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, SEG_OFF);
            HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, SEG_OFF);
            HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, SEG_OFF);
            HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, SEG_OFF);
            HAL_GPIO_WritePin(SEG4_GPIO_Port, SEG4_Pin, SEG_OFF);
            HAL_GPIO_WritePin(SEG5_GPIO_Port, SEG5_Pin, SEG_OFF);
            HAL_GPIO_WritePin(SEG6_GPIO_Port, SEG6_Pin, SEG_OFF); break;
    }
}

static inline void EN_AllOff(void){
    HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_SET);
}

static inline void EN_Select(int idx){
    switch(idx){
        case 0: HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, GPIO_PIN_RESET); break;
        case 1: HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_RESET); break;
        case 2: HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_RESET); break;
        case 3: HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_RESET); break;
    }
}

void update7SEG (int index) {
    EN_AllOff();
    display7SEG(led_buffer[index]);
    EN_Select(index);
}

void updateClockBuffer(void) {
    int h = hour % 24;
    int m = minute % 60;
    led_buffer[0] = h / 10;
    led_buffer[1] = h % 10;
    led_buffer[2] = m / 10;
    led_buffer[3] = m % 10;
}

void led_red(void){
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
}

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_TIM2_Init();

  updateClockBuffer();
  EN_AllOff();
  display7SEG(0);

  HAL_TIM_Base_Start_IT(&htim2);

  while (1) {
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
  HAL_GPIO_WritePin(GPIOB, SEG0_Pin|SEG1_Pin|SEG2_Pin|SEG3_Pin
                         |SEG4_Pin|SEG5_Pin|SEG6_Pin, GPIO_PIN_SET);

  GPIO_InitStruct.Pin = LED1_Pin|LED2_Pin|EN0_Pin|EN1_Pin|EN2_Pin|EN3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = SEG0_Pin|SEG1_Pin|SEG2_Pin|SEG3_Pin|SEG4_Pin|SEG5_Pin|SEG6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void Error_Handler(void)
{
  __disable_irq();
  while (1) {}
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line) { (void)file; (void)line; }
#endif
