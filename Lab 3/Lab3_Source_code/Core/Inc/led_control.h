/*
 * led_control.h
 *
 *  Created on: Oct 24, 2024
 *      Author: admin
 */

#ifndef INC_LED_CONTROL_H_
#define INC_LED_CONTROL_H_

unsigned char timer_flag_on(uint8_t led); //if timer_flag == 1, return true
void timer_run_road1(void); //a timer run for road 1
void timer_run_road2(void); //a timer run for road 2
void resetTimer(uint8_t led); //set new timer = global timer

void update7SEG(uint8_t enable); //scan 4 leds by enable 0 to 3
void updateLedBuffer(unsigned char enable0, unsigned char enable1, int value);
//new value for element of led buffer, en0 1st digit, en1 2nd digit
//each road has a couple digits

void timer_run_4digits(void);
//a timer for 4 digits led
//after set counter again

#endif /* INC_LED_CONTROL_H_ */
