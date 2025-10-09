/*
 * led_traffic_light.c
 *
 *  Created on: Oct 24, 2024
 *      Author: admin
 */

#include "main.h"

void ledOn(int led){ //0 - red, 1 - amber, 2 - green
	switch(led){
	case 0: // road 1 red
		HAL_GPIO_WritePin ( RED1_GPIO_Port, RED1_Pin , GPIO_PIN_RESET );
		HAL_GPIO_WritePin ( AMBER1_GPIO_Port, AMBER1_Pin, GPIO_PIN_SET );
		HAL_GPIO_WritePin ( GREEN1_GPIO_Port, GREEN1_Pin, GPIO_PIN_SET );
		break;
	case 1: //road 1 amber
		HAL_GPIO_WritePin ( RED1_GPIO_Port, RED1_Pin , GPIO_PIN_SET );
		HAL_GPIO_WritePin ( AMBER1_GPIO_Port, AMBER1_Pin, GPIO_PIN_RESET );
		HAL_GPIO_WritePin ( GREEN1_GPIO_Port, GREEN1_Pin, GPIO_PIN_SET );
		break;
	case 2: //road 1 green
		HAL_GPIO_WritePin ( RED1_GPIO_Port, RED1_Pin , GPIO_PIN_SET );
		HAL_GPIO_WritePin ( AMBER1_GPIO_Port, AMBER1_Pin, GPIO_PIN_SET );
		HAL_GPIO_WritePin ( GREEN1_GPIO_Port, GREEN1_Pin, GPIO_PIN_RESET );
		break;

	case 3: // road 2 red
		HAL_GPIO_WritePin ( RED2_GPIO_Port, RED2_Pin , GPIO_PIN_RESET );
		HAL_GPIO_WritePin ( AMBER2_GPIO_Port, AMBER2_Pin, GPIO_PIN_SET );
		HAL_GPIO_WritePin ( GREEN2_GPIO_Port, GREEN2_Pin, GPIO_PIN_SET );
		break;
	case 4: //road 2 amber
		HAL_GPIO_WritePin ( RED2_GPIO_Port, RED2_Pin , GPIO_PIN_SET );
		HAL_GPIO_WritePin ( AMBER2_GPIO_Port, AMBER2_Pin, GPIO_PIN_RESET );
		HAL_GPIO_WritePin ( GREEN2_GPIO_Port, GREEN2_Pin, GPIO_PIN_SET );
		break;
	case 5: //road 2 green
		HAL_GPIO_WritePin ( RED2_GPIO_Port, RED2_Pin , GPIO_PIN_SET );
		HAL_GPIO_WritePin ( AMBER2_GPIO_Port, AMBER2_Pin, GPIO_PIN_SET );
		HAL_GPIO_WritePin ( GREEN2_GPIO_Port, GREEN2_Pin, GPIO_PIN_RESET );
		break;
	default:
		HAL_GPIO_WritePin ( RED1_GPIO_Port, RED1_Pin , GPIO_PIN_SET );
		HAL_GPIO_WritePin ( AMBER1_GPIO_Port, AMBER1_Pin, GPIO_PIN_SET );
		HAL_GPIO_WritePin ( GREEN1_GPIO_Port, GREEN1_Pin, GPIO_PIN_SET );
		HAL_GPIO_WritePin ( RED2_GPIO_Port, RED2_Pin , GPIO_PIN_SET );
		HAL_GPIO_WritePin ( AMBER2_GPIO_Port, AMBER2_Pin, GPIO_PIN_SET );
		HAL_GPIO_WritePin ( GREEN2_GPIO_Port, GREEN2_Pin, GPIO_PIN_SET );
		break;
	}
}
