/*
 * button_control.c
 * Author: Quinn Ivison
 */
#include "stm32f3xx_hal.h"
#include "eeprom_control.h"
#include "button_control.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern void print(const char *str, uint16_t size);

static GPIO_TypeDef *OUT_PORT[NUM_COLUMNS] = {GPIOB, GPIOA, GPIOB};
static uint16_t OUT_PIN[NUM_COLUMNS] = {GPIO_PIN_3, GPIO_PIN_11, GPIO_PIN_5};
static GPIO_TypeDef *IN_PORT[NUM_ROWS] = {GPIOF, GPIOB, GPIOB, GPIOA};
static uint16_t IN_PIN[NUM_ROWS] = {GPIO_PIN_1, GPIO_PIN_1, GPIO_PIN_0, GPIO_PIN_12};

static char passcode[PASSCODE_SIZE] = {0, 0, 0, 0};



/* 											1 2 3
 * scanButton():							4 5 6
 * Scans button matrix for button presses	7 8 9
 * Returns read status and value			* 0 #
 */
bool scanButton(uint8_t* buttonValue)
{
	const uint8_t button_map[NUM_COLUMNS] = {BUTTON_LOCK, 0, BUTTON_EDIT};
	bool rtv = false;

	// Pull columns to digital HIGH
	for(uint8_t col = 0; col < NUM_COLUMNS; col++)
	{
		HAL_GPIO_WritePin(OUT_PORT[col], OUT_PIN[col], GPIO_PIN_SET);

		// Scan rows for button presses
		for(uint8_t row = 0; row < NUM_ROWS; row++)
		{
			if(HAL_GPIO_ReadPin(IN_PORT[row], IN_PIN[row]) == true)
			{
				//char msg[] = {row, col};
				//print(&msg);

				// Start timer for button timeout
				uint32_t timeout = __HAL_TIM_GET_COUNTER(&htim3) + BLINK_INTERVAL/2;
				while(__HAL_TIM_GET_COUNTER(&htim3) < timeout)
				{
					// Stall the MCU while the button is held to prevent multiple reads
					if (HAL_GPIO_ReadPin(IN_PORT[row], IN_PIN[row]) == false)
					{
						break;
					}
				}

				// Assign output value to button pressed
				if(row <  NUM_ROWS - 1)
				{
					rtv = true;
					*buttonValue = col * NUM_COLUMNS + row + 1;
				}
				else if(row == NUM_ROWS - 1)
				{
					rtv = true;
					*buttonValue = button_map[col];
				}

				// Disable column pulled high
				HAL_GPIO_WritePin(OUT_PORT[col], OUT_PIN[col], GPIO_PIN_RESET);
				goto breakLoop;
			}
		}

		// Disable column pulled high
		HAL_GPIO_WritePin(OUT_PORT[col], OUT_PIN[col], GPIO_PIN_RESET);
	}

	breakLoop:

	return rtv;
}


/*
 * actionSetPasscode():
 * Changes the passcode of the doorlock
 * Outputs success state
 */
uint8_t actionSetPasscode(void)
{
	static uint16_t index = 0;
	static char passcodeBuffer[PASSCODE_SIZE];

	uint8_t rtv = RETURN_NONE;

	// Check button matrix for input
	uint8_t buttonValue;
	if(scanButton(&buttonValue))
	{
		// Abort action if lock/edit button pressed
		if(buttonValue == BUTTON_LOCK || buttonValue == BUTTON_EDIT)
		{
			index = 0;
			rtv = buttonValue;
			HAL_TIM_Base_Stop(&htim1);
			__HAL_TIM_SET_COUNTER(&htim1, 0);
			memset(passcodeBuffer, 0, sizeof(passcodeBuffer));
		}
		else
		{
			// Add input to new passcode
			passcodeBuffer[index++] = buttonValue;

			// Check if passcode is full
			if(index == PASSCODE_SIZE)
			{
				index = 0;
				rtv = RETURN_SUCCESS;
				HAL_TIM_Base_Stop(&htim1);
				__HAL_TIM_SET_COUNTER(&htim1, 0);
				memcpy(passcode, passcodeBuffer, sizeof(char)*PASSCODE_SIZE);
				memset(passcodeBuffer, 0, sizeof(char)*PASSCODE_SIZE);

				writeEEPROM(passcode);
			}
			else
			{
				// Reset timeout timer
				rtv = RETURN_PENDING;
				HAL_TIM_Base_Start(&htim1);
				__HAL_TIM_SET_COUNTER(&htim1, 0);
			}
		}
	}
	else if(__HAL_TIM_GET_COUNTER(&htim1) > ACTION_TIMEOUT)
	{
		// Abort action if timeout activates
		index = 0;
		rtv = RETURN_FAILURE;
		HAL_TIM_Base_Stop(&htim1);
		__HAL_TIM_SET_COUNTER(&htim1, 0);
		memset(passcodeBuffer, 0, sizeof(passcodeBuffer));
	}

	return rtv;
}


/*
 * actionEnterPasscode():
 * Accept passcode to unlock the doorlock
 * Outputs success state
 */
uint8_t actionEnterPasscode(void)
{
	static uint8_t index = 0;
	static char passcodeBuffer[PASSCODE_SIZE];

	uint8_t rtv = RETURN_NONE;

	// Check button matrix for input
	uint8_t buttonValue;
	if(scanButton(&buttonValue))
	{
		// Abort action if lock/edit button pressed
		if(buttonValue == BUTTON_LOCK || buttonValue == BUTTON_EDIT)
		{
			index = 0;
			rtv = buttonValue;
			HAL_TIM_Base_Stop(&htim1);
			__HAL_TIM_SET_COUNTER(&htim1, 0);
			memset(passcodeBuffer, 0, sizeof(passcodeBuffer));
		}
		else
		{
			// Add new input to passcode entry
			passcodeBuffer[index++] = buttonValue;

			// Check if passcode entry is full
			if(index == PASSCODE_SIZE)
			{
				index = 0;
				rtv = RETURN_SUCCESS;

				// Check if passcode entry is correct
				for(uint8_t i = 0; i < PASSCODE_SIZE; i++)
				{
					rtv &= passcode[i] == passcodeBuffer[i];
				}

				HAL_TIM_Base_Stop(&htim1);
				__HAL_TIM_SET_COUNTER(&htim1, 0);
				memset(passcodeBuffer, 0, sizeof(char)*PASSCODE_SIZE);
			}
			else
			{
				// Reset timeout timer
				rtv = RETURN_PENDING;
				HAL_TIM_Base_Start(&htim1);
				__HAL_TIM_SET_COUNTER(&htim1, 0);
			}
		}
	}
	else if(__HAL_TIM_GET_COUNTER(&htim1) > ACTION_TIMEOUT)
	{
		// Abort action if timeout activates
		index = 0;
		rtv = RETURN_FAILURE;
		HAL_TIM_Base_Stop(&htim1);
		__HAL_TIM_SET_COUNTER(&htim1, 0);
		memset(passcodeBuffer, 0, sizeof(passcodeBuffer));
	}

	return rtv;
}
