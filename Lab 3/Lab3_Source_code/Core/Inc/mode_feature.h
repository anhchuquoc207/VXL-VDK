/*
 * mode_feature.h
 *
 *  Created on: Oct 25, 2024
 *      Author: admin
 */

#ifndef INC_MODE_FEATURE_H_
#define INC_MODE_FEATURE_H_

void changeModeState(void);
//switch each mode (use button 1)

void ledOff(void);
//turn off all 6 led traffic

void timer_run_mode(void);
//4 digit scan

unsigned char timer_mode_flag_on(uint8_t mode);
void currentModeRun(void);
void mode2State(void);
void mode3State(void);
void mode4State(void);

void timer_run_4digits_mode(void);

void button_2_changed_value(void);
void button_2_value(int newValue);

void setValue(void);

#endif /* INC_MODE_FEATURE_H_ */
