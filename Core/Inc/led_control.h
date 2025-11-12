/*
 * led_control.h
 * Author: Quinn Ivison
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define BLINK_INTERVAL		500 	    // 500 ms

#define ERROR_TIMEOUT		5000	    // 5000 ms
#define PENDING_TIMEOUT		MAX_TIMEOUT	// 65535 ms
#define SUCCESS_TIMEOUT		5000	    // 5000 ms
#define MAX_TIMEOUT			~0

#define NUM_LEDS		4
#define NUM_STATUS_LEDS 3

typedef enum
{
	eNONE,
	eERROR,
	ePENDING,
	eSUCCESS
} e_ledType;


void errorLED(void);
void successLED(void);
void pendingLED(void);

void activateLED(e_ledType ledType);
void disableLED(void);
void blinkLED(void);

