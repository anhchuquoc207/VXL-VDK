/*
 * global.c
 *
 *  Created on: Oct 24, 2024
 *      Author: admin
 */
#include "global.h"


int timer_counter[N0_OF_TIMERS] = {
	INITIAL_RED_LED_TIME * TIME_SCALE_COUNTER,
	INITIAL_AMBER_LED_TIME * TIME_SCALE_COUNTER,
	INITIAL_GREEN_LED_TIME  * TIME_SCALE_COUNTER,
	INITIAL_RED_LED_TIME * TIME_SCALE_COUNTER,
	INITIAL_AMBER_LED_TIME * TIME_SCALE_COUNTER,
	INITIAL_GREEN_LED_TIME * TIME_SCALE_COUNTER,
	COUNTER_4DIGITS_LED,
	COUNTER_4DIGITS_LED
};
int timer_flag[N0_OF_TIMERS] = {0};  // assuming all flags start at 0
int led_buffer[N0_LED_7SEGMENT] = {0, 0, 0, 0};

void setTimer(unsigned char index, int duration) {
	timer_counter[index] = duration / TIMER_CYCLE ;
	timer_flag[index] = 0;
}

enum LedColorState ledColorStateRoad1 = RED_LED; //led light state of road 1
enum LedColorState ledColorStateRoad2 = GREEN_LED; //led light state of road 2

int RED_LED_COUNTER = INITIAL_RED_LED_TIME * TIME_SCALE_MS;    //global variable red light counter
int AMBER_LED_COUNTER = INITIAL_AMBER_LED_TIME * TIME_SCALE_MS;  //global variable amber light counter
int GREEN_LED_COUNTER = INITIAL_GREEN_LED_TIME * TIME_SCALE_MS;  //global variable green light counter


int DIGITS_LED_COUNTER = 250;

enum ModeState currentMode = MODE0; //initial mode state (indefinite)

int addValue = 0; //handle the times click button 2
