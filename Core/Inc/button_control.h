/*
 * button_control.h
 * Author: Quinn Ivison
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"

#define NUM_ROWS 			    4
#define NUM_COLUMNS 		    3

#define BUTTON_TIMEOUT		    1000// 1s
#define ACTION_TIMEOUT		    10000// 10s
#define BUTTON_DEBOUNCE_TIMEOUT 50

typedef enum
{
	BUTTON_LOCK = 0x7f,
	BUTTON_EDIT
} e_buttonMap;

typedef enum
{
	RETURN_FAILURE = 0,
	RETURN_SUCCESS,
	RETURN_PENDING,
	RETURN_NONE
} e_btnReturnState;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;

extern void print(const char *str, uint16_t size);

void getPasscode(uint8_t* buffer);
void setPasscode(uint8_t* buffer);

bool readButton(uint8_t row, uint8_t column);
bool scanButtons(uint8_t* buttonValue);
e_btnReturnState actionSetPasscode(void);
e_btnReturnState actionEnterPasscode(void);
