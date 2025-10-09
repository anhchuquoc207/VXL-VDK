/*
 * fsm_auto.c
 *
 *  Created on: Oct 24, 2024
 *      Author: admin
 */


#include "main.h"
#include "global.h"
#include "led_control.h"


void fsm_auto(void) {
	if (currentMode == MODE1){
    switch(ledColorStateRoad1){
        case RED_LED:
            if(timer_flag_on(0)) {
            	ledColorStateRoad1 = GREEN_LED;
            }
            ledOn(0);
            break;
        case AMBER_LED:
            if (timer_flag_on(1)) {
            	ledColorStateRoad1 = RED_LED;
            }
            ledOn(1);
            break;
        case GREEN_LED:
        	if (timer_flag_on(2)) {
        		ledColorStateRoad1 = AMBER_LED;
        	}
        	ledOn(2);
            break;
    }

    switch(ledColorStateRoad2){
        case RED_LED:
            if(timer_flag_on(3)) {
            	ledColorStateRoad2 = GREEN_LED;
            }
            ledOn(3);
            break;
        case AMBER_LED:
            if (timer_flag_on(4)) {
            	ledColorStateRoad2 = RED_LED;
            }
            ledOn(4);
            break;
        case GREEN_LED:
        	if (timer_flag_on(5)) {
        		ledColorStateRoad2 = AMBER_LED;
        	}
        	ledOn(5);
            break;
    }
	}
}
