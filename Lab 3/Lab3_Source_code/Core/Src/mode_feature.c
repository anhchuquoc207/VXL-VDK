/*
 * mode_feature.c
 *
 *  Created on: Oct 25, 2024
 *      Author: admin
 */


#include "main.h"
#include "global.h"
#include "fsm_auto.h"
#include "led_7segment.h"
#include "button.h"

#define MAX_VALUE 99

void ledOff(void){
	HAL_GPIO_WritePin(RED1_GPIO_Port, RED1_Pin, SET);
	HAL_GPIO_WritePin(AMBER1_GPIO_Port, AMBER1_Pin, SET);
	HAL_GPIO_WritePin(GREEN1_GPIO_Port, GREEN1_Pin, SET);
	HAL_GPIO_WritePin(RED2_GPIO_Port, RED2_Pin, SET);
	HAL_GPIO_WritePin(AMBER2_GPIO_Port, AMBER2_Pin, SET);
	HAL_GPIO_WritePin(GREEN2_GPIO_Port, GREEN2_Pin, SET);
}

void button_2_value(int newValue){
	led_buffer[0] = 0;

	switch(currentMode){
	case MODE2:
		led_buffer[1] = 2;
		break;
	case MODE3:
		led_buffer[1] = 3;
		break;
	case MODE4:
		led_buffer[1] = 4;
		break;
	default:
		break;
	}
	newValue = newValue / TIME_SCALE_MS;
	led_buffer[2] = newValue / 10;
	led_buffer[3] = newValue % 10;
}

void changeModeState(void){
	setTimer(7, DIGITS_LED_COUNTER);
	addValue = 0;
	switch(currentMode){
	case MODE1:
		ledOff();
		currentMode = MODE2;
		break;
	case MODE2:
		ledOff();
		currentMode = MODE3;
		break;
	case MODE3:
		ledOff();
		currentMode = MODE4;
		break;
	case MODE4:
		ledOff();
		currentMode = MODE1;
		break;
	default:
		ledOff();
		currentMode = MODE1;
		break;
	}
}

void timer_run_mode(void){
	if (timer_counter[7] > 0){
		timer_counter[7]--;
		if (timer_counter[7] <= 0) timer_flag[7] = 1;
	}
}

unsigned char timer_mode_flag_on(uint8_t mode){
	if (mode >= N0_OF_TIMERS) return 0;
	if (timer_flag[mode] == 1){
		setTimer(7, DIGITS_LED_COUNTER);
		return 1;
	}
	return 0;
}



void mode2State(void){
	if (timer_mode_flag_on(7) && currentMode == MODE2){
		HAL_GPIO_TogglePin(RED1_GPIO_Port, RED1_Pin);
		HAL_GPIO_TogglePin(RED2_GPIO_Port, RED2_Pin);
	}
}

void mode3State(void){
	if (timer_mode_flag_on(7) && currentMode == MODE3){
		HAL_GPIO_TogglePin(AMBER1_GPIO_Port, AMBER1_Pin);
		HAL_GPIO_TogglePin(AMBER2_GPIO_Port, AMBER2_Pin);
	}
}

void mode4State(void){
	if (timer_mode_flag_on(7) && currentMode == MODE4){
		HAL_GPIO_TogglePin(GREEN1_GPIO_Port, GREEN1_Pin);
		HAL_GPIO_TogglePin(GREEN2_GPIO_Port, GREEN2_Pin);
	}
}

void currentModeRun(void){
	switch(currentMode){
	case MODE2:
		mode2State();
		break;
	case MODE3:
		mode3State();
		break;
	case MODE4:
		mode4State();
		break;
	default:
		break;
	}
}

int led_enable_mode = 0;

void update7SEG_mode(uint8_t enable) {
    enableMode(enable);
    display7SEG(led_buffer[enable]);
}

void button_2_changed_value(void){
	int newValue = 0;
	int maxAdd = 0;
	switch(currentMode){
	case MODE2:
		newValue = RED_LED_COUNTER + addValue * TIME_SCALE_MS;
		maxAdd = MAX_VALUE - RED_LED_COUNTER / TIME_SCALE_MS;
		break;
	case MODE3:
		newValue = AMBER_LED_COUNTER + addValue * TIME_SCALE_MS;
		maxAdd = MAX_VALUE - AMBER_LED_COUNTER / TIME_SCALE_MS;
		break;
	case MODE4:
		newValue = GREEN_LED_COUNTER + addValue * TIME_SCALE_MS;
		maxAdd = MAX_VALUE - GREEN_LED_COUNTER / TIME_SCALE_MS;
		break;
	default:
		break;
	}


	if (newValue > MAX_VALUE * TIME_SCALE_MS) {
		addValue = maxAdd - MAX_VALUE + 1;
		newValue = 1000;
	}

	button_2_value(newValue);
}

void setValue(void){
	switch(currentMode){
	case MODE2:
		timer_counter[0] = timer_counter[0] + addValue * TIME_SCALE_COUNTER;
		timer_counter[3] = timer_counter[3] + addValue * TIME_SCALE_COUNTER;
		RED_LED_COUNTER = RED_LED_COUNTER + addValue * TIME_SCALE_MS;
		break;
	case MODE3:
		timer_counter[1] = timer_counter[1] + addValue * TIME_SCALE_COUNTER;
		timer_counter[4] = timer_counter[4] + addValue * TIME_SCALE_COUNTER;
		AMBER_LED_COUNTER = AMBER_LED_COUNTER + addValue * TIME_SCALE_MS;
		break;
	case MODE4:
		timer_counter[2] = timer_counter[2] + addValue * TIME_SCALE_COUNTER;
		timer_counter[5] = timer_counter[5] + addValue * TIME_SCALE_COUNTER;
		GREEN_LED_COUNTER = GREEN_LED_COUNTER + addValue * TIME_SCALE_MS;
		break;
	default:
		break;
	}
	addValue = 0;
}

void timer_run_4digits_mode(void){
	if(timer_counter[6] > 0){
		timer_counter[6]--;
		if (timer_counter[6] <= 0) {
			button_2_changed_value();
			update7SEG_mode(led_enable_mode++);
			if (led_enable_mode >= 4) led_enable_mode = 0;
			setTimer(6, DIGITS_LED_COUNTER);
		}
	}
}
