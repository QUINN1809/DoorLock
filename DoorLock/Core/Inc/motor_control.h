/*
 * motor_control.h
 * Author: Quinn Ivison
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define STEP_DELAY		10

#define MOTOR_FORWARD	0
#define MOTOR_REVERSE	1

void actionLock(void);
void actionUnlock(void);
void powerMotor(uint16_t direction, uint16_t steps);
