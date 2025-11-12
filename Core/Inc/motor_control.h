/*
 * motor_control.h
 * Author: Quinn Ivison
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// DRV8825 Pins
#define MOTOR_ENABLE_PORT   (GPIOB)
#define MOTOR_ENABLE_PIN    (GPIO_PIN_4)
#define MOTOR_STEP_PORT     (GPIOA)
#define MOTOR_STEP_PIN      (GPIO_PIN_6)
#define MOTOR_DIR_PORT      (GPIOA)
#define MOTOR_DIR_PIN       (GPIO_PIN_5)

#define MOTOR_STEP_DELAY		1	// 1 ms


void powerMotor(bool direction, uint16_t steps);
