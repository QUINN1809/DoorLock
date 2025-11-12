/*
 * led_control.c
 * Author: Quinn Ivison
 */

#include "stm32f3xx_hal.h"
#include "led_control.h"

extern TIM_HandleTypeDef htim3;

static e_ledType activeLED = eNONE;
static bool activeLEDState = false;

static uint16_t LED_TIMEOUTS[NUM_LEDS] = {MAX_TIMEOUT, ERROR_TIMEOUT, PENDING_TIMEOUT, SUCCESS_TIMEOUT};
static GPIO_TypeDef *LED_PORTS[NUM_LEDS] = {GPIOA, GPIOA, GPIOA, GPIOA};
static uint16_t LED_PINS[NUM_LEDS] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_3, GPIO_PIN_4};


/*
 * errorLED():
 * Activates error status LED
 * Inputs:
 *      (None)
 * Outputs:
 *      (None)
 */
void errorLED(void)
{
	activateLED(eERROR);
}


/*
 * pendingLED():
 * Activates idle status LED
 * Inputs:
 *      (None)
 * Outputs:
 *      (None)
 */
void pendingLED(void)
{
	activateLED(ePENDING);
}


/*
 * successLED():
 * Activates success status LED
 * Inputs:
 *      (None)
 * Outputs:
 *      (None)
 */
void successLED(void)
{
	activateLED(eSUCCESS);
}


/*
 * activateLED():
 * Activates status LEDs
 * Inputs:
 *      (ledType) Type of status LED to enable
 * Outputs:
 *      (None)
 */
void activateLED(e_ledType ledType)
{
	if(activeLED == ledType)
	{
		return;
	}

	disableLED();
	activeLED = ledType;
}


/*
 * disableLED():
 * Disables active status LEDs
 * Inputs:
 *      (None)
 * Outputs:
 *      (None)
 */
void disableLED(void)
{
	if(activeLED == eNONE)
	{
		return;
	}

	HAL_GPIO_WritePin(LED_PORTS[activeLED - 1], LED_PINS[activeLED - 1], GPIO_PIN_RESET);
	activeLEDState = false;
	activeLED = eNONE;
}


/*
 * blinkLED():
 * Blinks active LED periodically
 * Inputs:
 *      (None)
 * Outputs:
 *      (None)
 */
void blinkLED(void)
{
	if(activeLED == eNONE)
	{
		return;
	}

	// Disable LED if timeout activates
	if(__HAL_TIM_GET_COUNTER(&htim3) > LED_TIMEOUTS[activeLED])
	{
		disableLED();
	}
	// Activate LED
	else if(((__HAL_TIM_GET_COUNTER(&htim3) % BLINK_INTERVAL) < (BLINK_INTERVAL / 2)) && !activeLEDState)
	{
		HAL_GPIO_WritePin(LED_PORTS[activeLED - 1], LED_PINS[activeLED - 1], GPIO_PIN_SET);
		activeLEDState = true;
	}
	// Disable LED
	else if(((__HAL_TIM_GET_COUNTER(&htim3) % BLINK_INTERVAL) >= (BLINK_INTERVAL / 2)) && activeLEDState)
	{
		HAL_GPIO_WritePin(LED_PORTS[activeLED - 1], LED_PINS[activeLED - 1], GPIO_PIN_RESET);
		activeLEDState = false;
	}
}
