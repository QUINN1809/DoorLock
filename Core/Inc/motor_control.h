/*
 * motor_control.h
 * Author: Quinn Ivison
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define STEP_DELAY		1	// 1 ms

void powerMotor(bool direction, uint16_t steps);
