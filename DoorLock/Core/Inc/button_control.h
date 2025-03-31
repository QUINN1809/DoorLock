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

#define BLINK_INTERVAL		500 	// 500 ms
#define ACTION_TIMEOUT		10000	// 10  s

typedef enum
{
	BUTTON_LOCK = 0x7f,
	BUTTON_EDIT
} buttonMap;

typedef enum
{
	RETURN_FAILURE,
	RETURN_SUCCESS,
	RETURN_PENDING,
	RETURN_NONE
} actionReturn;

bool scanButton(uint8_t* buttonValue);
uint8_t actionSetPasscode(void);
uint8_t actionEnterPasscode(void);
