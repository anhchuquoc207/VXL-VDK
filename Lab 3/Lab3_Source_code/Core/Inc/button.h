/*
 * button.h
 *
 *  Created on: Oct 24, 2024
 *      Author: admin
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

void debounceButtonBuffer1_pin(void);
//if there are more than 1 button
//this func assign all button pin to element of buffer array

void button_reading(void);
//read the state of button

unsigned char is_button_pressed(unsigned char index);
//is button press?

unsigned char is_button_pressed_1s(unsigned char index);
//is button press more than 1s?

#endif /* INC_BUTTON_H_ */
