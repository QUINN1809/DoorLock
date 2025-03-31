/*
 * led_control.c
 * Author: Quinn Ivison
 */

#include "stm32f3xx_hal.h"
#include "led_control.h"

extern TIM_HandleTypeDef htim3;

static uint16_t activeLED = eNONE;
static bool activeLEDState = false;

static uint16_t LED_TIMEOUTS[NUM_LEDS] = {MAX_TIMEOUT, ERROR_TIMEOUT, PENDING_TIMEOUT, SUCCESS_TIMEOUT};
static GPIO_TypeDef *LED_PORTS[NUM_LEDS] = {GPIOA, GPIOA, GPIOA, GPIOA};
static uint16_t LED_PINS[NUM_LEDS] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_3, GPIO_PIN_4};



void errorLED(void)
{
	activateLED(eERROR);
}


void pendingLED(void)
{
	activateLED(ePENDING);
}


void successLED(void)
{
	activateLED(eSUCCESS);
}


void activateLED(uint16_t ledType)
{
	uint32_t time = __HAL_TIM_GET_COUNTER(&htim3) % BLINK_INTERVAL;
	__HAL_TIM_SET_COUNTER(&htim3, time);

	if(activeLED == ledType)
	{
		return;
	}

	disableLED();
	activeLED = ledType;
}


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
