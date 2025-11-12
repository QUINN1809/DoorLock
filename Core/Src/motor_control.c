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
 * Inputs:
 *      (direction) direction of motor torque
 *      (steps) amount of steps to move motor
 * Outputs:
 *      (None)
 */
void powerMotor(bool direction, uint16_t steps)
{
    // Activate power to motor controller (active low)
    HAL_GPIO_WritePin(MOTOR_ENABLE_PORT, MOTOR_ENABLE_PIN, GPIO_PIN_RESET);

	// Set motor direction
	HAL_GPIO_WritePin(MOTOR_DIR_PORT, MOTOR_DIR_PIN, direction);

	// Drive motor steps
	for (uint16_t i = 0; i < steps; i++)
	{
		HAL_GPIO_WritePin(MOTOR_STEP_PORT, MOTOR_STEP_PIN, GPIO_PIN_SET);
		HAL_Delay(MOTOR_STEP_DELAY);
		HAL_GPIO_WritePin(MOTOR_STEP_PORT, MOTOR_STEP_PIN, GPIO_PIN_RESET);
		HAL_Delay(MOTOR_STEP_DELAY);
	}

	// Gate power to motor controller
	HAL_GPIO_WritePin(MOTOR_ENABLE_PORT, MOTOR_ENABLE_PIN, GPIO_PIN_SET);
}
