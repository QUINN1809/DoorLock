/*
 * led_control.c
 * Author: Quinn Ivison
 */

#include "stm32f3xx_hal.h"
#include "led_control.h"

uint16_t activeLED = eNONE;
bool activeLEDState = false;

uint16_t timeoutLED = 0;

GPIO_TypeDef *LED_PORTS[NUM_LEDS] = {GPIOA, GPIOA, GPIOA, GPIOA};
uint16_t LED_PINS[NUM_LEDS] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_3, GPIO_PIN_4};

void initLEDTimer(TIM_HandleTypeDef timer)
{
	htim3 = timer;
}

void errorLED(void)
{
	activateLED(eERROR, 5000);
}

void pendingLED(void)
{
	activateLED(ePENDING, 0xffff);
}

void successLED(void)
{
	activateLED(eSUCCESS, 5000);
}

void activateLED(uint16_t ledType, uint16_t timeout)
{
	timeoutLED = timeout;
	uint32_t time = __HAL_TIM_GET_COUNTER(&htim3) % BLINK_INTERVAL;
	__HAL_TIM_SET_COUNTER(&htim3, time);

	if(activeLED == ledType)
	{
		return;
	}

	disableLED();
	HAL_TIM_Base_Start(&htim3);
	activeLEDState = false;
	activeLED = ledType;
}

void disableLED(void)
{
	HAL_GPIO_WritePin(LED_PORTS[activeLED - 1], LED_PINS[activeLED - 1], GPIO_PIN_RESET);
	HAL_TIM_Base_Stop(&htim3);

	activeLEDState = false;
	activeLED = eNONE;
}

void blinkLED(void)
{
	if(activeLED == eNONE)
	{
		return;
	}

	if(__HAL_TIM_GET_COUNTER(&htim3) > timeoutLED)
	{
		disableLED();
	}
	else if(((__HAL_TIM_GET_COUNTER(&htim3) % BLINK_INTERVAL) < (BLINK_INTERVAL / 2)) && !activeLEDState)
	{
		HAL_GPIO_WritePin(LED_PORTS[activeLED - 1], LED_PINS[activeLED - 1], GPIO_PIN_SET);
		activeLEDState = true;
	}
	else if(((__HAL_TIM_GET_COUNTER(&htim3) % BLINK_INTERVAL) >= (BLINK_INTERVAL / 2)) && activeLEDState)
	{
		HAL_GPIO_WritePin(LED_PORTS[activeLED - 1], LED_PINS[activeLED - 1], GPIO_PIN_RESET);
		activeLEDState = false;
	}
}
