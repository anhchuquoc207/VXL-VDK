#include "main.h"

#define a_GPIO_Port GPIOB
#define b_GPIO_Port GPIOB
#define c_GPIO_Port GPIOB
#define d_GPIO_Port GPIOB
#define e_GPIO_Port GPIOB
#define f_GPIO_Port GPIOB
#define g_GPIO_Port GPIOB

#define a_Pin GPIO_PIN_0
#define b_Pin GPIO_PIN_1
#define c_Pin GPIO_PIN_2
#define d_Pin GPIO_PIN_3
#define e_Pin GPIO_PIN_4
#define f_Pin GPIO_PIN_5
#define g_Pin GPIO_PIN_6

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void display7SEG(int num);

static void display7SEG (int num){
  switch (num){
    case 0:
      HAL_GPIO_WritePin (a_GPIO_Port, a_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (b_GPIO_Port, b_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (c_GPIO_Port, c_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (d_GPIO_Port, d_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (e_GPIO_Port, e_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (f_GPIO_Port, f_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (g_GPIO_Port, g_Pin, GPIO_PIN_SET);
      break ;
    case 1:
      HAL_GPIO_WritePin (a_GPIO_Port, a_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin (b_GPIO_Port, b_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (c_GPIO_Port, c_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (d_GPIO_Port, d_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin (e_GPIO_Port, e_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin (f_GPIO_Port, f_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin (g_GPIO_Port, g_Pin, GPIO_PIN_SET);
      break ;
    case 2:
      HAL_GPIO_WritePin (a_GPIO_Port, a_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (b_GPIO_Port, b_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (c_GPIO_Port, c_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin (d_GPIO_Port, d_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (e_GPIO_Port, e_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (f_GPIO_Port, f_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin (g_GPIO_Port, g_Pin, GPIO_PIN_RESET);
      break ;
    case 3:
      HAL_GPIO_WritePin (a_GPIO_Port, a_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (b_GPIO_Port, b_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (c_GPIO_Port, c_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (d_GPIO_Port, d_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (e_GPIO_Port, e_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin (f_GPIO_Port, f_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin (g_GPIO_Port, g_Pin, GPIO_PIN_RESET);
      break ;
    case 4:
      HAL_GPIO_WritePin (a_GPIO_Port, a_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin (b_GPIO_Port, b_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (c_GPIO_Port, c_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (d_GPIO_Port, d_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin (e_GPIO_Port, e_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin (f_GPIO_Port, f_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (g_GPIO_Port, g_Pin, GPIO_PIN_RESET);
      break ;
    case 5:
      HAL_GPIO_WritePin (a_GPIO_Port, a_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (b_GPIO_Port, b_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin (c_GPIO_Port, c_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (d_GPIO_Port, d_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (e_GPIO_Port, e_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin (f_GPIO_Port, f_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (g_GPIO_Port, g_Pin, GPIO_PIN_RESET);
      break ;
    case 6:
      HAL_GPIO_WritePin (a_GPIO_Port, a_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (b_GPIO_Port, b_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin (c_GPIO_Port, c_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (d_GPIO_Port, d_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (e_GPIO_Port, e_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (f_GPIO_Port, f_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (g_GPIO_Port, g_Pin, GPIO_PIN_RESET);
      break ;
    case 7:
      HAL_GPIO_WritePin (a_GPIO_Port, a_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (b_GPIO_Port, b_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (c_GPIO_Port, c_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (d_GPIO_Port, d_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin (e_GPIO_Port, e_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin (f_GPIO_Port, f_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin (g_GPIO_Port, g_Pin, GPIO_PIN_SET);
      break ;
    case 8:
      HAL_GPIO_WritePin (a_GPIO_Port, a_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (b_GPIO_Port, b_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (c_GPIO_Port, c_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (d_GPIO_Port, d_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (e_GPIO_Port, e_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (f_GPIO_Port, f_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (g_GPIO_Port, g_Pin, GPIO_PIN_RESET);
      break ;
    case 9:
      HAL_GPIO_WritePin (a_GPIO_Port, a_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (b_GPIO_Port, b_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (c_GPIO_Port, c_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (d_GPIO_Port, d_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (e_GPIO_Port, e_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin (f_GPIO_Port, f_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin (g_GPIO_Port, g_Pin, GPIO_PIN_RESET);
      break;
    default :
      HAL_GPIO_WritePin (a_GPIO_Port, a_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin (b_GPIO_Port, b_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin (c_GPIO_Port, c_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin (d_GPIO_Port, d_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin (e_GPIO_Port, e_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin (f_GPIO_Port, f_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin (g_GPIO_Port, g_Pin, GPIO_PIN_SET);
      break ;
  }
}

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  int counter = 0;

  HAL_GPIO_WritePin (a_GPIO_Port, a_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin (b_GPIO_Port, b_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin (c_GPIO_Port, c_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin (d_GPIO_Port, d_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin (e_GPIO_Port, e_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin (f_GPIO_Port, f_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin (g_GPIO_Port, g_Pin, GPIO_PIN_SET);

  while (1)
  {
    if (counter >= 10) counter = 0;
    display7SEG(counter++);
    HAL_Delay(1000);

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

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitStruct.Pin = a_Pin | b_Pin | c_Pin | d_Pin | e_Pin | f_Pin | g_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOB, a_Pin | b_Pin | c_Pin | d_Pin | e_Pin | f_Pin | g_Pin, GPIO_PIN_SET);
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif
