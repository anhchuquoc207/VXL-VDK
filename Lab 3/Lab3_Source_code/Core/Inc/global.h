/*
 * global.h
 *
 *  Created on: Oct 24, 2024
 *      Author: admin
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_


#define TIMER_CYCLE 10
#define N0_OF_TIMERS 8
#define N0_LED_7SEGMENT 4
#define INITIAL_RED_LED_TIME 5
#define INITIAL_AMBER_LED_TIME 2
#define INITIAL_GREEN_LED_TIME 3
#define COUNTER_4DIGITS_LED 25 //2 hz

#define TIME_SCALE_COUNTER 100
#define TIME_SCALE_MS 1000

extern int timer_counter[N0_OF_TIMERS];
extern int timer_flag[N0_OF_TIMERS];
extern int led_buffer[N0_LED_7SEGMENT];

void setTimer(unsigned char index, int duration);

enum LedColorState{RED_LED, AMBER_LED, GREEN_LED};
extern enum LedColorState ledColorStateRoad1;
extern enum LedColorState ledColorStateRoad2;

extern int RED_LED_COUNTER;
extern int AMBER_LED_COUNTER;
extern int GREEN_LED_COUNTER;

extern int DIGITS_LED_COUNTER;

enum ModeState{MODE1, MODE2, MODE3, MODE4, MODE0};
extern enum ModeState currentMode;

extern int addValue;

#endif /* INC_GLOBAL_H_ */
