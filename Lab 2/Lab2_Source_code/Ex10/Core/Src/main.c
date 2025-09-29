#include "main.h"

#include "main.h"

/* ---- General / constants ---- */
#define MAX_LED_MATRIX   8

/* ---- On-board LED + DOT ---- */
#define LED1_GPIO_Port   GPIOA
#define LED1_Pin         GPIO_PIN_5
#define DOT_GPIO_Port    GPIOA
#define DOT_Pin          GPIO_PIN_4

/* ---- 7-seg digit enables (common enable lines) ---- */
#define EN0_GPIO_Port    GPIOA
#define EN0_Pin          GPIO_PIN_6
#define EN1_GPIO_Port    GPIOA
#define EN1_Pin          GPIO_PIN_7
#define EN2_GPIO_Port    GPIOA
#define EN2_Pin          GPIO_PIN_8
#define EN3_GPIO_Port    GPIOA
#define EN3_Pin          GPIO_PIN_9

/* ---- 7-seg segments A..G on GPIOB ----
   SEG0..SEG6 correspond to segments A..G in your display7SEG() */
#define SEG0_GPIO_Port   GPIOB  // A
#define SEG0_Pin         GPIO_PIN_0
#define SEG1_GPIO_Port   GPIOB  // B
#define SEG1_Pin         GPIO_PIN_1
#define SEG2_GPIO_Port   GPIOB  // C
#define SEG2_Pin         GPIO_PIN_2
#define SEG3_GPIO_Port   GPIOB  // D
#define SEG3_Pin         GPIO_PIN_3
#define SEG4_GPIO_Port   GPIOB  // E
#define SEG4_Pin         GPIO_PIN_4
#define SEG5_GPIO_Port   GPIOB  // F
#define SEG5_Pin         GPIO_PIN_5
#define SEG6_GPIO_Port   GPIOB  // G
#define SEG6_Pin         GPIO_PIN_6

/* Segment polarity: RESET = ON, SET = OFF (matches your display7SEG) */
#define SEG_ON           GPIO_PIN_RESET
#define SEG_OFF          GPIO_PIN_SET

/* ---- LED Matrix rows on GPIOB: ROW0..ROW7 = PB8..PB15 ---- */
#define ROW0_GPIO_Port   GPIOB
#define ROW0_Pin         GPIO_PIN_8
#define ROW1_GPIO_Port   GPIOB
#define ROW1_Pin         GPIO_PIN_9
#define ROW2_GPIO_Port   GPIOB
#define ROW2_Pin         GPIO_PIN_10
#define ROW3_GPIO_Port   GPIOB
#define ROW3_Pin         GPIO_PIN_11
#define ROW4_GPIO_Port   GPIOB
#define ROW4_Pin         GPIO_PIN_12
#define ROW5_GPIO_Port   GPIOB
#define ROW5_Pin         GPIO_PIN_13
#define ROW6_GPIO_Port   GPIOB
#define ROW6_Pin         GPIO_PIN_14
#define ROW7_GPIO_Port   GPIOB
#define ROW7_Pin         GPIO_PIN_15

/* ---- LED Matrix column enables on GPIOA (active LOW) ----
   ENM0..ENM7 = PA2, PA3, PA10..PA15 (matches MX_GPIO_Init) */
#define ENM0_GPIO_Port   GPIOA
#define ENM0_Pin         GPIO_PIN_2
#define ENM1_GPIO_Port   GPIOA
#define ENM1_Pin         GPIO_PIN_3
#define ENM2_GPIO_Port   GPIOA
#define ENM2_Pin         GPIO_PIN_10
#define ENM3_GPIO_Port   GPIOA
#define ENM3_Pin         GPIO_PIN_11
#define ENM4_GPIO_Port   GPIOA
#define ENM4_Pin         GPIO_PIN_12
#define ENM5_GPIO_Port   GPIOA
#define ENM5_Pin         GPIO_PIN_13
#define ENM6_GPIO_Port   GPIOA
#define ENM6_Pin         GPIO_PIN_14
#define ENM7_GPIO_Port   GPIOA
#define ENM7_Pin         GPIO_PIN_15

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
int hour = 15, minute = 8, second = 59;

const int MAX_LED = 4;
int index_led = 0;
int led_buffer[4];

uint8_t matrix_buffer[MAX_LED_MATRIX] = {0xFF, 0xFF, 0x83, 0xED, 0xED, 0x83, 0xFF, 0xFF};
int index_led_matrix = 0;


/* Pin mapping cho Matrix */
uint16_t ROW_Pins[MAX_LED_MATRIX] = {
ROW0_Pin, ROW1_Pin, ROW2_Pin, ROW3_Pin,
ROW4_Pin, ROW5_Pin, ROW6_Pin, ROW7_Pin
};

GPIO_TypeDef* ROW_Ports[MAX_LED_MATRIX] = {
ROW0_GPIO_Port, ROW1_GPIO_Port, ROW2_GPIO_Port, ROW3_GPIO_Port,
ROW4_GPIO_Port, ROW5_GPIO_Port, ROW6_GPIO_Port, ROW7_GPIO_Port
};

uint16_t ENM_Pins[MAX_LED_MATRIX] = {
ENM0_Pin, ENM1_Pin, ENM2_Pin, ENM3_Pin,
ENM4_Pin, ENM5_Pin, ENM6_Pin, ENM7_Pin
};

GPIO_TypeDef* ENM_Ports[MAX_LED_MATRIX] = {
ENM0_GPIO_Port, ENM1_GPIO_Port, ENM2_GPIO_Port, ENM3_GPIO_Port,
ENM4_GPIO_Port, ENM5_GPIO_Port, ENM6_GPIO_Port, ENM7_GPIO_Port
};

/* Software timers */
int timer0_counter = 0; // 1s timer (đếm giây)
int timer0_flag = 0;
int timer1_counter = 0; // quét LED
int timer1_flag = 0;

int counterMatrix = 0, flagMatrix = 0;
int counterAnim = 0, flagAnim = 0;

int TIMER_CYCLE = 10; // timer interrupt period = 10ms

void setTimer0(int duration){
timer0_counter = duration / TIMER_CYCLE;
timer0_flag = 0;
}

void setTimer1(int duration){
timer1_counter = duration / TIMER_CYCLE;
timer1_flag = 0;
}

void setTimerMatrix (int duration){
counterMatrix = duration/ TIMER_CYCLE;
flagMatrix = 0;
}

void setTimerAnim (int duration){
counterAnim = duration / TIMER_CYCLE;
flagAnim = 0;
}

void timer_run(){
if(timer0_counter > 0){
timer0_counter--;
if(timer0_counter == 0) timer0_flag = 1;
}
if(timer1_counter > 0){
timer1_counter--;
if(timer1_counter == 0) timer1_flag = 1;
}

if(counterMatrix>0){ counterMatrix--; if(counterMatrix==0) flagMatrix=1; }
if(counterAnim>0){ counterAnim--; if(counterAnim==0) flagAnim=1; }
}

void updateClockBuffer(){
led_buffer[0] = hour / 10;
led_buffer[1] = hour % 10;
led_buffer[2] = minute / 10;
led_buffer[3] = minute % 10;
}

void display7SEG(int num) {
    // Turn off all segments
    HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, GPIO_PIN_SET); // A
    HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, GPIO_PIN_SET); // B
    HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, GPIO_PIN_SET); // C
    HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, GPIO_PIN_SET); // D
    HAL_GPIO_WritePin(SEG4_GPIO_Port, SEG4_Pin, GPIO_PIN_SET); // E
    HAL_GPIO_WritePin(SEG5_GPIO_Port, SEG5_Pin, GPIO_PIN_SET); // F
    HAL_GPIO_WritePin(SEG6_GPIO_Port, SEG6_Pin, GPIO_PIN_SET); // G

    switch(num) {
        case 0:
            HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, GPIO_PIN_RESET); // A
            HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, GPIO_PIN_RESET); // B
            HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, GPIO_PIN_RESET); // C
            HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, GPIO_PIN_RESET); // D
            HAL_GPIO_WritePin(SEG4_GPIO_Port, SEG4_Pin, GPIO_PIN_RESET); // E
            HAL_GPIO_WritePin(SEG5_GPIO_Port, SEG5_Pin, GPIO_PIN_RESET); // F
            break;
        case 1:
            HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, GPIO_PIN_RESET); // B
            HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, GPIO_PIN_RESET); // C
            break;
        case 2:
            HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, GPIO_PIN_RESET); // A
            HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, GPIO_PIN_RESET); // B
            HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, GPIO_PIN_RESET); // D
            HAL_GPIO_WritePin(SEG4_GPIO_Port, SEG4_Pin, GPIO_PIN_RESET); // E
            HAL_GPIO_WritePin(SEG6_GPIO_Port, SEG6_Pin, GPIO_PIN_RESET); // G
            break;
        case 3:
            HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, GPIO_PIN_RESET); // A
            HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, GPIO_PIN_RESET); // B
            HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, GPIO_PIN_RESET); // C
            HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, GPIO_PIN_RESET); // D
            HAL_GPIO_WritePin(SEG6_GPIO_Port, SEG6_Pin, GPIO_PIN_RESET); // G
            break;
        case 4:
            HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, GPIO_PIN_RESET); // B
            HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, GPIO_PIN_RESET); // C
            HAL_GPIO_WritePin(SEG5_GPIO_Port, SEG5_Pin, GPIO_PIN_RESET); // F
            HAL_GPIO_WritePin(SEG6_GPIO_Port, SEG6_Pin, GPIO_PIN_RESET); // G
            break;
        case 5:
            HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, GPIO_PIN_RESET); // A
            HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, GPIO_PIN_RESET); // C
            HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, GPIO_PIN_RESET); // D
            HAL_GPIO_WritePin(SEG5_GPIO_Port, SEG5_Pin, GPIO_PIN_RESET); // F
            HAL_GPIO_WritePin(SEG6_GPIO_Port, SEG6_Pin, GPIO_PIN_RESET); // G
            break;
        case 6:
            HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, GPIO_PIN_RESET); // A
            HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, GPIO_PIN_RESET); // C
            HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, GPIO_PIN_RESET); // D
            HAL_GPIO_WritePin(SEG4_GPIO_Port, SEG4_Pin, GPIO_PIN_RESET); // E
            HAL_GPIO_WritePin(SEG5_GPIO_Port, SEG5_Pin, GPIO_PIN_RESET); // F
            HAL_GPIO_WritePin(SEG6_GPIO_Port, SEG6_Pin, GPIO_PIN_RESET); // G
            break;
        case 7:
            HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, GPIO_PIN_RESET); // A
            HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, GPIO_PIN_RESET); // B
            HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, GPIO_PIN_RESET); // C
            break;
        case 8:
            HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, GPIO_PIN_RESET); // A
            HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, GPIO_PIN_RESET); // B
            HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, GPIO_PIN_RESET); // C
            HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, GPIO_PIN_RESET); // D
            HAL_GPIO_WritePin(SEG4_GPIO_Port, SEG4_Pin, GPIO_PIN_RESET); // E
            HAL_GPIO_WritePin(SEG5_GPIO_Port, SEG5_Pin, GPIO_PIN_RESET); // F
            HAL_GPIO_WritePin(SEG6_GPIO_Port, SEG6_Pin, GPIO_PIN_RESET); // G
            break;
        case 9:
            HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, GPIO_PIN_RESET); // A
            HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, GPIO_PIN_RESET); // B
            HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, GPIO_PIN_RESET); // C
            HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, GPIO_PIN_RESET); // D
            HAL_GPIO_WritePin(SEG5_GPIO_Port, SEG5_Pin, GPIO_PIN_RESET); // F
            HAL_GPIO_WritePin(SEG6_GPIO_Port, SEG6_Pin, GPIO_PIN_RESET); // G
            break;
    }
}



void update7SEG(int index){
switch(index){
case 0:
HAL_GPIO_WritePin(GPIOA, EN1_Pin, SET);
HAL_GPIO_WritePin(GPIOA, EN2_Pin, SET);
HAL_GPIO_WritePin(GPIOA, EN3_Pin, SET);
display7SEG(led_buffer[0]);
HAL_GPIO_WritePin(GPIOA, EN0_Pin, RESET);
break;
case 1:
HAL_GPIO_WritePin(GPIOA, EN0_Pin, SET);
HAL_GPIO_WritePin(GPIOA, EN2_Pin, SET);
HAL_GPIO_WritePin(GPIOA, EN3_Pin, SET);
display7SEG(led_buffer[1]);
HAL_GPIO_WritePin(GPIOA, EN1_Pin, RESET);
break;
case 2:
HAL_GPIO_WritePin(GPIOA, EN0_Pin, SET);
HAL_GPIO_WritePin(GPIOA, EN1_Pin, SET);
HAL_GPIO_WritePin(GPIOA, EN3_Pin, SET);
display7SEG(led_buffer[2]);
HAL_GPIO_WritePin(GPIOA, EN2_Pin, RESET);
break;
case 3:
HAL_GPIO_WritePin(GPIOA, EN0_Pin, SET);
HAL_GPIO_WritePin(GPIOA, EN1_Pin, SET);
HAL_GPIO_WritePin(GPIOA, EN2_Pin, SET);
display7SEG(led_buffer[3]);
HAL_GPIO_WritePin(GPIOA, EN3_Pin, RESET);
break;
default:
break;
}
}

/* ---- Bài 9: updateLEDMatrix ---- */
void disableAllColumns(void){
for(int i=0;i<MAX_LED_MATRIX;i++){
HAL_GPIO_WritePin(ENM_Ports[i], ENM_Pins[i], GPIO_PIN_SET);
}
}

void updateLEDMatrix(int index){
disableAllColumns();
uint8_t colData = matrix_buffer[index];
for(int row=0; row<MAX_LED_MATRIX; row++){
if(colData & (1<<row)){
HAL_GPIO_WritePin(ROW_Ports[row], ROW_Pins[row], GPIO_PIN_SET);
} else {
HAL_GPIO_WritePin(ROW_Ports[row], ROW_Pins[row], GPIO_PIN_RESET);
}
}
HAL_GPIO_WritePin(ENM_Ports[index], ENM_Pins[index], GPIO_PIN_RESET);
}

/* ---- Bài 10: animation ---- */
void shiftMatrixLeftOnce(void){
uint8_t tmp = matrix_buffer[0];
for(int i=0;i<7;i++) matrix_buffer[i]=matrix_buffer[i+1];
matrix_buffer[7]= tmp;
}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);
  setTimer0(100);
  setTimer1(25);
  setTimerMatrix(10);
  setTimerAnim(100);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//  if(timer0_flag){
//  timer0_flag = 0;
//  setTimer0(100);
//
//  second++;
//  if(second >= 60){ second=0; minute++; }
//  if(minute >= 60){ minute=0; hour++; }
//  if(hour >= 24) hour=0;
//
//  updateClockBuffer();
//  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4); // DOT nhấp nháy
//  }
//
//  // Quét LED
//  if(timer1_flag){
//  timer1_flag = 0;
//  setTimer1(25);
//
//  update7SEG(index_led);
//  index_led++;
//  if(index_led >= MAX_LED) index_led = 0;
//  }

 /* ---- Refresh Matrix ---- */
 if(flagMatrix){
 flagMatrix=0;
 setTimerMatrix(10);
 updateLEDMatrix(index_led_matrix);
 index_led_matrix++;
 if(index_led_matrix>=MAX_LED_MATRIX) index_led_matrix=0;
 }

 /* ---- Animation Matrix ---- */
 if(flagAnim){
  flagAnim=0;
  setTimerAnim(100);
  shiftMatrixLeftOnce();
  }
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

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, ENM0_Pin|ENM1_Pin|DOT_Pin|EN0_Pin
                          |EN1_Pin|EN2_Pin|EN3_Pin|ENM2_Pin
                          |ENM3_Pin|ENM4_Pin|ENM5_Pin|ENM6_Pin
                          |ENM7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SEG0_Pin|SEG1_Pin|SEG2_Pin|ROW2_Pin
                          |ROW3_Pin|ROW4_Pin|ROW5_Pin|ROW6_Pin
                          |ROW7_Pin|SEG3_Pin|SEG4_Pin|SEG5_Pin
                          |SEG6_Pin|ROW0_Pin|ROW1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : ENM0_Pin ENM1_Pin DOT_Pin EN0_Pin
                           EN1_Pin EN2_Pin EN3_Pin ENM2_Pin
                           ENM3_Pin ENM4_Pin ENM5_Pin ENM6_Pin
                           ENM7_Pin */
  GPIO_InitStruct.Pin = ENM0_Pin|ENM1_Pin|DOT_Pin|EN0_Pin
                          |EN1_Pin|EN2_Pin|EN3_Pin|ENM2_Pin
                          |ENM3_Pin|ENM4_Pin|ENM5_Pin|ENM6_Pin
                          |ENM7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : SEG0_Pin SEG1_Pin SEG2_Pin ROW2_Pin
                           ROW3_Pin ROW4_Pin ROW5_Pin ROW6_Pin
                           ROW7_Pin SEG3_Pin SEG4_Pin SEG5_Pin
                           SEG6_Pin ROW0_Pin ROW1_Pin */
  GPIO_InitStruct.Pin = SEG0_Pin|SEG1_Pin|SEG2_Pin|ROW2_Pin
                          |ROW3_Pin|ROW4_Pin|ROW5_Pin|ROW6_Pin
                          |ROW7_Pin|SEG3_Pin|SEG4_Pin|SEG5_Pin
                          |SEG6_Pin|ROW0_Pin|ROW1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
timer_run();
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
