/*
 * motor_control.c
 * Author: Quinn Ivison
 */

#include "stm32f3xx_hal.h"
#include "motor_control.h"

void actionLock(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, MOTOR_FORWARD);
	powerMotor(MOTOR_FORWARD, 100U);
}

void actionUnlock(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, MOTOR_REVERSE);
	powerMotor(MOTOR_REVERSE, 100U);
}

void powerMotor(uint16_t direction, uint16_t steps)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);

	for (uint16_t i = 0; i < steps; i++)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
		HAL_Delay(STEP_DELAY);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
		HAL_Delay(STEP_DELAY);
	}
}
