/*
 * button_control.c
 * Author: Quinn Ivison
 */
#include "stm32f3xx_hal.h"
#include "eeprom_control.h"
#include "button_control.h"

static GPIO_TypeDef* OUT_PORT[NUM_COLUMNS] = {GPIOB, GPIOA, GPIOB};
static uint16_t OUT_PIN[NUM_COLUMNS] = {GPIO_PIN_3, GPIO_PIN_11, GPIO_PIN_5};
static GPIO_TypeDef* IN_PORT[NUM_ROWS] = {GPIOF, GPIOB, GPIOB, GPIOA};
static uint16_t IN_PIN[NUM_ROWS] = {GPIO_PIN_1, GPIO_PIN_1, GPIO_PIN_0, GPIO_PIN_12};

static uint8_t passcode[PASSCODE_SIZE] = {0, 0, 0, 0};
static uint32_t button_timers[NUM_ROWS][NUM_COLUMNS] = {0};


/*
 * getPasscode():
 * returns the keypad passcode
 * Inputs:
 *      (None)
 * Outputs:
 *      (buffer) Keypad passcode
 */
void getPasscode(uint8_t* buffer)
{
    for(uint8_t i = 0; i<PASSCODE_SIZE; i++)
    {
        buffer[i] = passcode[i];
    }
}


/*
 * setPasscode():
 * sets the internal keypad passcode
 * Inputs:
 *      (buffer) Keypad passcode
 * Outputs:
 *      (None)
 */
void setPasscode(uint8_t* buffer)
{
    for(uint8_t i = 0; i<PASSCODE_SIZE; i++)
    {
        passcode[i] = buffer[i];
    }
}

/*
 * readButton():
 * Reads button and debounces the signal
 * Inputs:
 *      (row) button matrix row
 *      (column) button matrix column
 * Outputs:
 *      (return value) button state
 */
bool readButton(uint8_t row, uint8_t column)
{
    if( __HAL_TIM_GET_COUNTER(&htim3) - button_timers[row][column] > BUTTON_DEBOUNCE_TIMEOUT)
    {
        return false;
    }

    button_timers[row][column] = __HAL_TIM_GET_COUNTER(&htim3);
    bool state = HAL_GPIO_ReadPin(IN_PORT[row], IN_PIN[row]);

    if(state)
    {
        // Start timer for button timeout
        uint32_t timeout = __HAL_TIM_GET_COUNTER(&htim3) + BUTTON_TIMEOUT;
        while(__HAL_TIM_GET_COUNTER(&htim3) < timeout)
        {
            // Stall the MCU while the button is held to prevent multiple reads
            if (!HAL_GPIO_ReadPin(IN_PORT[row], IN_PIN[row]))
            {
                break;
            }
        }
    }

    return state;
}


/* 											1 2 3
 * scanButtons():							4 5 6
 * Scans button matrix for button presses	7 8 9
 * Inputs:                                  * 0 #
 *      (None)
 * Outputs:
 *      (buttonValue) button identifier
 *      (return value) read status
 */
bool scanButtons(uint8_t* button)
{
	uint8_t button_map[NUM_ROWS][NUM_COLUMNS] = {{1, 2, 3},
                                                 {4, 5, 6},
                                                 {7, 8, 9},
                                                 {BUTTON_LOCK, 0, BUTTON_EDIT}};

	bool break_loop = false;
	bool rtv = false;

	// Pull columns to digital LOW
	for(uint8_t col = 0; col < NUM_COLUMNS; col++)
	{
		HAL_GPIO_WritePin(OUT_PORT[col], OUT_PIN[col], GPIO_PIN_RESET);

		// Scan rows for button presses
		for(uint8_t row = 0; row < NUM_ROWS; row++)
		{
			if(readButton(row, col))
			{
                rtv = true;

				// Assign output value to button pressed
				*button = button_map[row][col];

				// Exit button scan
				break_loop = true;
				break;
			}
		}

		// Enable column pulled low
		HAL_GPIO_WritePin(OUT_PORT[col], OUT_PIN[col], GPIO_PIN_SET);

		if(break_loop)
		{
		    break;
		}
	}

	return rtv;
}


/*
 * actionSetPasscode():
 * Detects and handles passcode changes initiated by the user
 * Inputs:
 *      (None)
 * Outputs:
 *      (return value) State of passcode change
 */
e_btnReturnState actionSetPasscode(void)
{
	static uint16_t index = 0;
	static char passcodeBuffer[PASSCODE_SIZE];

	e_btnReturnState rtv = RETURN_NONE;

	// Check button matrix for input
	uint8_t buttonValue;
	if(scanButtons(&buttonValue))
	{
		// Abort action if lock/edit button pressed
		if(buttonValue == BUTTON_LOCK || buttonValue == BUTTON_EDIT)
		{
			index = 0;
			rtv = buttonValue;
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
				__HAL_TIM_SET_COUNTER(&htim1, 0);
				memcpy(passcode, passcodeBuffer, sizeof(char)*PASSCODE_SIZE);
				memset(passcodeBuffer, 0, sizeof(char)*PASSCODE_SIZE);
			}
			else
			{
				// Reset timeout timer
				rtv = RETURN_PENDING;
				__HAL_TIM_SET_COUNTER(&htim1, 0);
			}
		}
	}
	else if(__HAL_TIM_GET_COUNTER(&htim1) > ACTION_TIMEOUT)
	{
		// Abort action if timeout activates
		index = 0;
		rtv = RETURN_FAILURE;
		__HAL_TIM_SET_COUNTER(&htim1, 0);
		memset(passcodeBuffer, 0, sizeof(passcodeBuffer));
	}

	return rtv;
}


/*
 * actionEnterPasscode():
 * Detects and validates passcode entered by user
 * Inputs:
 *      (None)
 * Outputs:
 *      (return value) State of passcode validation
 */
e_btnReturnState actionEnterPasscode(void)
{
	static uint8_t index = 0;
	static char passcodeBuffer[PASSCODE_SIZE];

	e_btnReturnState rtv = RETURN_NONE;

	// Check button matrix for input
	uint8_t buttonValue;
	if(scanButtons(&buttonValue))
	{
		// Abort action if lock/edit button pressed
		if(buttonValue == BUTTON_LOCK || buttonValue == BUTTON_EDIT)
		{
			index = 0;
			rtv = buttonValue;
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

				__HAL_TIM_SET_COUNTER(&htim1, 0);
				memset(passcodeBuffer, 0, sizeof(char)*PASSCODE_SIZE);
			}
			else
			{
				// Reset timeout timer
				rtv = RETURN_PENDING;
				__HAL_TIM_SET_COUNTER(&htim1, 0);
			}
		}
	}
	else if(__HAL_TIM_GET_COUNTER(&htim1) > ACTION_TIMEOUT)
	{
		// Abort action if timeout activates
		index = 0;
		rtv = RETURN_FAILURE;
		__HAL_TIM_SET_COUNTER(&htim1, 0);
		memset(passcodeBuffer, 0, sizeof(passcodeBuffer));
	}

	return rtv;
}
