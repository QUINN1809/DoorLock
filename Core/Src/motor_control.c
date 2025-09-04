/*
 * motor_control.c
 * Author: Quinn Ivison
 */

#include "stm32f3xx_hal.h"
#include "motor_control.h"



/*
 * powerMotor():
 * Drives the stepper motor in the given direction
 * by the given amount of steps
 */
void powerMotor(bool direction, uint16_t steps)
{
	// Set motor direction
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, direction);

	// Drive motor steps
	for (uint16_t i = 0; i < steps; i++)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
		HAL_Delay(STEP_DELAY);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
		HAL_Delay(STEP_DELAY);
	}
}
