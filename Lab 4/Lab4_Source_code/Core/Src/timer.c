#include "main.h"
#include "sched.h"

void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim ) {
	if(htim->Instance == TIM2){
		SCH_Update();
	}
}
