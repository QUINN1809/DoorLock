/*
 * button_control.h
 * Author: Quinn Ivison
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_ROWS 			4
#define NUM_COLUMNS 		3

#define PASSCODE_SIZE 		4

#define ACTION_TIMEOUT		10000

typedef enum
{
	RETURN_NONE = 0xff,
	RETURN_LOCK,
	RETURN_EDIT
} buttonReturn;

typedef enum
{
	RETURN_FAILURE,
	RETURN_SUCCESS
} actionReturn;

typedef enum
{
	ePASSIVE,
	eLOCKING,
	eUNLOCKING,
	eSETTING_PASSCODE
} lockState;

extern TIM_HandleTypeDef htim1;
extern UART_HandleTypeDef huart1;

extern GPIO_TypeDef *OUT_PORT[NUM_COLUMNS];
extern uint16_t OUT_PIN[NUM_COLUMNS];

extern GPIO_TypeDef *IN_PORT[NUM_ROWS];
extern uint16_t IN_PIN[NUM_ROWS];

extern char passcode[PASSCODE_SIZE];

void setButtonTimer(TIM_HandleTypeDef timer);
void setUARTHandle(UART_HandleTypeDef uart);

uint16_t scanButton(void);
uint16_t actionSetPasscode(void);
uint16_t actionEnterPasscode(void);
