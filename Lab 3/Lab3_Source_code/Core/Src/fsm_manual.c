/*
 * fsm_manual.c
 *
 *  Created on: Oct 24, 2024
 *      Author: admin
 */


#include "main.h"
#include "button.h"
#include "mode_feature.h"
#include "global.h"

enum ButtonState{BUTTON_RELEASED, BUTTON_PRESSED, BUTTON_PRESSED_MORE_THAN_1_SECOND};
enum ButtonState buttonState1 = BUTTON_RELEASED;
enum ButtonState buttonState2 = BUTTON_RELEASED;
enum ButtonState buttonState3 = BUTTON_RELEASED;

void fsm_manual(void) {
	currentModeRun();
    switch(buttonState1){
        case BUTTON_RELEASED:
            if(is_button_pressed(0)) {
                buttonState1 = BUTTON_PRESSED;
                // INCREASE VALUE OF PORT A BY ONE UNIT
                changeModeState();
            }
            break;
        case BUTTON_PRESSED:
            if (!is_button_pressed(0)) {
                buttonState1 = BUTTON_RELEASED;
            } else {
                if(is_button_pressed_1s(0)) {
                    buttonState1 = BUTTON_PRESSED_MORE_THAN_1_SECOND;
                }
            }

            break;
        case BUTTON_PRESSED_MORE_THAN_1_SECOND:
            if (!is_button_pressed(0)) {
                buttonState1 = BUTTON_RELEASED;
            }
            // todo
            break;
	}

    switch(buttonState2){
        case BUTTON_RELEASED:
            if(is_button_pressed(1)) {
                buttonState2 = BUTTON_PRESSED;
                // INCREASE VALUE OF PORT A BY ONE UNIT
                addValue++;
            }
            break;
        case BUTTON_PRESSED:
            if (!is_button_pressed(1)) {
                buttonState2 = BUTTON_RELEASED;
            } else {
                if(is_button_pressed_1s(1)) {
                    buttonState2 = BUTTON_PRESSED_MORE_THAN_1_SECOND;
                }
            }

            break;
        case BUTTON_PRESSED_MORE_THAN_1_SECOND:
            if (!is_button_pressed(1)) {
                buttonState2 = BUTTON_RELEASED;
            }
            // todo
            break;
	}

    switch(buttonState3){
        case BUTTON_RELEASED:
            if(is_button_pressed(2)) {
                buttonState3 = BUTTON_PRESSED;
                // INCREASE VALUE OF PORT A BY ONE UNIT
                //increaseTime();
                setValue();
            }
            break;
        case BUTTON_PRESSED:
            if (!is_button_pressed(2)) {
                buttonState3 = BUTTON_RELEASED;
            } else {
                if(is_button_pressed_1s(2)) {
                    buttonState3 = BUTTON_PRESSED_MORE_THAN_1_SECOND;
                }
            }

            break;
        case BUTTON_PRESSED_MORE_THAN_1_SECOND:
            if (!is_button_pressed(2)) {
                buttonState3 = BUTTON_RELEASED;
            }
            // todo
            break;
	}
}
