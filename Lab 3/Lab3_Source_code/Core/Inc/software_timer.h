/*
 * software_timer.h
 *
 *  Created on: Oct 24, 2024
 *      Author: admin
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim);
//timer interupt
//call every 10ms
//pre scale 7999 - counter 9

#endif /* INC_SOFTWARE_TIMER_H_ */
