#include "main.h"
//#include "timer.h"

#define LED_OFF GPIO_PIN_RESET
#define LED_ON GPIO_PIN_SET
#define LED0 0
#define LED1 1
#define LED2 2
#define LED3 3
#define LED4 4


void setUp(void){
	HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, LED_OFF);
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, LED_OFF);
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, LED_OFF);
	HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, LED_OFF);
	HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, LED_OFF);

}

void blinkyLed0(){
		HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
}

void blinkyLed1(){
		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
}

void blinkyLed2(){
		HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
}

void blinkyLed3(){
		HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
}

void blinkyLed4(){
		HAL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin);
}
