/*
 * software_timer.c
 *
 *  Created on: Oct 24, 2024
 *      Author: admin
 */


#include "main.h"
#include "button.h"
#include "led_control.h"
#include "global.h"
#include "mode_feature.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim)
{
    if(htim->Instance == TIM2){
        button_reading();

        if (currentMode == MODE1){
        	timer_run_road1();

        	timer_run_road2();

        	timer_run_4digits();
        }

        if (currentMode == MODE2 || currentMode == MODE3 || currentMode == MODE4){
        	timer_run_mode();
        	//setTimer(6, DIGITS_LED_COUNTER);
        	timer_run_4digits_mode();
        }

    }
}
