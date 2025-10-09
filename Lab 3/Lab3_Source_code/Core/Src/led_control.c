/*
 * led_control.c
 *
 *  Created on: Oct 24, 2024
 *      Author: admin
 */


#include "main.h"
#include "led_traffic_light.h"
#include "software_timer.h"
#include "global.h"
#include "led_7segment.h"

void timer_run_road1(void){
	if(timer_counter[0] > 0 && ledColorStateRoad1 == RED_LED) {
		timer_counter[0]--;
		if(timer_counter[0] <= 0) timer_flag[0] = 1;
		updateLedBuffer(0, 1, timer_counter[0]);
	}
	if(timer_counter[1] > 0 && ledColorStateRoad1 == AMBER_LED) {
		timer_counter[1]--;
		if(timer_counter[1] <= 0) timer_flag[1] = 1;
		updateLedBuffer(0, 1, timer_counter[1]);
	}
	if(timer_counter[2] > 0 && ledColorStateRoad1 == GREEN_LED) {
		timer_counter[2]--;
		if(timer_counter[2] <= 0) timer_flag[2] = 1;
		updateLedBuffer(0, 1, timer_counter[2]);
	}
}

void timer_run_road2(void){
	if(timer_counter[3] > 0 && ledColorStateRoad2 == RED_LED) {
		timer_counter[3]--;
		if(timer_counter[3] <= 0) timer_flag[3] = 1;
		updateLedBuffer(2, 3, timer_counter[3]);
	}
	if(timer_counter[4] > 0 && ledColorStateRoad2 == AMBER_LED) {
		timer_counter[4]--;
		if(timer_counter[4] <= 0) timer_flag[4] = 1;
		updateLedBuffer(2, 3, timer_counter[4]);
	}
	if(timer_counter[5] > 0 && ledColorStateRoad2 == GREEN_LED) {
		timer_counter[5]--;
		if(timer_counter[5] <= 0) timer_flag[5] = 1;
		updateLedBuffer(2, 3, timer_counter[5]);
	}
}

void resetTimer(uint8_t led){
	switch(led){
	case 0:
		setTimer(led, RED_LED_COUNTER);
		break;
	case 1:
		setTimer(led, AMBER_LED_COUNTER);
		break;
	case 2:
		setTimer(led, GREEN_LED_COUNTER);
		break;
	case 3:
		setTimer(led, RED_LED_COUNTER);
		break;
	case 4:
		setTimer(led, AMBER_LED_COUNTER);
		break;
	case 5:
		setTimer(led, GREEN_LED_COUNTER);
		break;
	}
}

unsigned char timer_flag_on(uint8_t led){
	if (led >= N0_OF_TIMERS) return 0;
	if (timer_flag[led] == 1){
		resetTimer(led);
		return 1;
	}
	return 0;
}

int led_enable = 0;

void updateLedBuffer(unsigned char enable0, unsigned char enable1, int value){
	value = value / 100; //counter / 100
	led_buffer[enable0] = value / 10;
	led_buffer[enable1] = value % 10;
}

void update7SEG(uint8_t enable) {
    enableMode(enable);
    display7SEG(led_buffer[enable]);
}

void timer_run_4digits(void){
	if(timer_counter[6] > 0){
		timer_counter[6]--;
		if (timer_counter[6] <= 0) {
			update7SEG(led_enable++);
			if (led_enable >= 4) led_enable = 0;
			setTimer(6, DIGITS_LED_COUNTER);
		}
	}
}
