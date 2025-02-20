/*
 * button_control.c
 * Author: Quinn Ivison
 */
#include "stm32f3xx_hal.h"

#include "led_control.h"
#include "eeprom_control.h"
#include "motor_control.h"
#include "button_control.h"

GPIO_TypeDef *OUT_PORT[NUM_COLUMNS] = {GPIOB, GPIOA, GPIOB};
uint16_t OUT_PIN[NUM_COLUMNS] = {GPIO_PIN_3, GPIO_PIN_11, GPIO_PIN_5};

GPIO_TypeDef *IN_PORT[NUM_ROWS] = {GPIOF, GPIOB, GPIOB, GPIOA};
uint16_t IN_PIN[NUM_ROWS] = {GPIO_PIN_1, GPIO_PIN_1, GPIO_PIN_0, GPIO_PIN_12};

char passcode[PASSCODE_SIZE];

void setButtonTimer(TIM_HandleTypeDef timer)
{
	htim1 = timer;
}

void setUARTHandle(UART_HandleTypeDef uart)
{
	huart1 = uart;
}

uint16_t scanButton(void)
{
	uint16_t button = RETURN_NONE;

	for(uint8_t col = 0; col < NUM_COLUMNS; col++)
	{
		//Write columns high
		HAL_GPIO_WritePin(OUT_PORT[col], OUT_PIN[col], GPIO_PIN_SET);
		HAL_Delay(5);

		for(uint8_t row = 0; row < NUM_ROWS; row++)
		{
			if(HAL_GPIO_ReadPin(IN_PORT[row], IN_PIN[row]) == true)
			{
				HAL_TIM_Base_Start(&htim1);
				__HAL_TIM_SET_COUNTER(&htim1, 0);
				while(1)
				{
					if (HAL_GPIO_ReadPin(IN_PORT[row], IN_PIN[row]) == false || __HAL_TIM_GET_COUNTER(&htim1) > ACTION_TIMEOUT)
					{
						break;
					}
				}
				HAL_TIM_Base_Stop(&htim1);

				if(row <  NUM_ROWS - 1)
				{
					button = col * NUM_COLUMNS + row + 1;
				}
				else if(row == NUM_ROWS - 1)
				{
					const uint16_t button_map[NUM_COLUMNS] = {RETURN_LOCK, 0, RETURN_EDIT};
					button = button_map[col];
				}

				HAL_GPIO_WritePin(OUT_PORT[col], OUT_PIN[col], GPIO_PIN_RESET);
				goto breakLoop;
			}
		}

		HAL_GPIO_WritePin(OUT_PORT[col], OUT_PIN[col], GPIO_PIN_RESET);
	}

	breakLoop:

	HAL_Delay(50);
	return button;
}

uint16_t actionSetPasscode(void)
{
	uint16_t rtv = RETURN_NONE;
	static uint16_t index = 0;
	static char passcodeBuffer[PASSCODE_SIZE];

	uint16_t num = scanButton();
	if(__HAL_TIM_GET_COUNTER(&htim1) > ACTION_TIMEOUT || num == RETURN_LOCK || num == RETURN_EDIT)
	{
		index = 0;
		HAL_TIM_Base_Stop(&htim1);
		__HAL_TIM_SET_COUNTER(&htim1, 0);
		memset(passcodeBuffer, 0, sizeof(passcodeBuffer));

		rtv = num;
	}
	else if(num != RETURN_NONE)
	{
		passcode[index] = num;
		index++;

		if(index == PASSCODE_SIZE)
		{

			writeEEPROM(passcode);

			index = 0;
			HAL_TIM_Base_Stop(&htim1);
			memcpy(passcode, passcodeBuffer, sizeof(char)*PASSCODE_SIZE);
			memset(passcodeBuffer, 0, sizeof(char)*PASSCODE_SIZE);
			rtv = RETURN_SUCCESS;
		}
		else
		{
			if(index == 1)
			{
				HAL_TIM_Base_Start(&htim1);
			}

			__HAL_TIM_SET_COUNTER(&htim1, 0);
		}
	}

	return rtv;
}

uint16_t actionEnterPasscode(void)
{
	uint16_t rtv = RETURN_NONE;
	static uint16_t index = 0;
	static char passcodeBuffer[PASSCODE_SIZE];

	uint16_t num = scanButton();
	if(num != RETURN_NONE && num != RETURN_LOCK && num != RETURN_EDIT)
	{
		passcodeBuffer[index] = num;
		index++;

		if(index == PASSCODE_SIZE)
		{
			rtv = RETURN_SUCCESS;
			for(uint8_t i = 0; i < PASSCODE_SIZE; i++)
			{
				rtv &= passcode[i] == passcodeBuffer[i];
			}

			index = 0;
			HAL_TIM_Base_Stop(&htim1);
			memset(passcodeBuffer, 0, sizeof(char)*PASSCODE_SIZE);

			if(rtv == RETURN_SUCCESS)
			{
				successLED();
			}
			else
			{
				errorLED();
			}
		}
		else
		{
			if(index == 1)
			{
				HAL_TIM_Base_Start(&htim1);
				pendingLED();
			}

			__HAL_TIM_SET_COUNTER(&htim1, 0);
		}
	}
	else if(__HAL_TIM_GET_COUNTER(&htim1) > ACTION_TIMEOUT || num == RETURN_LOCK || num == RETURN_EDIT)
	{
		index = 0;
		memset(passcodeBuffer, 0, sizeof(passcodeBuffer));
		HAL_TIM_Base_Stop(&htim1);

		disableLED();
		rtv = num;
		__HAL_TIM_SET_COUNTER(&htim1, 0);
	}

	return rtv;
}
