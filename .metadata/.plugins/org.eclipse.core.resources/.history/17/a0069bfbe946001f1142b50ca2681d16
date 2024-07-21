/*
 * buttonmatrix.c
 *
 *  Created on: Jul 11, 2024
 *      Author: Quinn
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "button_control.h"

int scanButton()
{
	int button = ~0;

	sleep(50);
	for(int col = 0; col < NUM_COLUMNS; col++)
	{
		//Write columns high

		for(int row = 0; row < NUM_ROWS; row++)
		{
			if(/*Read row*/)
			{
				while(/*Read row*/);

				if(row < 3)
				{
					button = col * 3 + row + 1;
				}
				else
				{
					switch(col)
					{
						case 0:
							button = RETURN_LOCK;
							break;

						case 1:
							button = 0;
							break;

						case 2:
							button = RETURN_SET;
							break;

						default:
							break;
					}
				}

				goto breakLoop;
			}
		}
	}

	breakLoop:

	return button;
}

bool actionSetPasscode()
{
	bool rtv = true;
	int num = 0;
	int index = 0;
	char passcodeTemp[PASSCODE_SIZE];
	while(1)
	{
		num = scanButton();
		if(num != RETURN_NONE && num != RETURN_LOCK && num != RETURN_EDIT_CODE)
		{
			passcode[index] = num;
			index++;
			if(index == PASSCODE_SIZE)
			{
				break;
			}
		}

		if(/*Timeout*/)
		{
			rtv = false;
			break;
		}
	}

	//Write Passcode to EEPROM
	passcode = passcodeTemp;

	return rtv;
}

bool actionEnterPasscode()
{
	static int index = 0;
	static char passcodeTemp[PASSCODE_SIZE];

	int num = scanButton();
	if(num != RETURN_NONE && num != RETURN_LOCK && num != RETURN_EDIT_CODE)
	{
		passcodeTemp[index] = num;
		index++;

		if(index == PASSCODE_SIZE)
		{
			for(int i = 0; i < PASSCODE_SIZE; i++)
			{
				if(passcode[i] != passcodeTemp[i])
				{
					memset(passcodeTemp, 0, sizeof(passcodeTemp));
					index = 0;
					//reset timer
				}
			}

			//Passcode matches

			//Unlock The Doorlock
		}
	}


	//Write Passcode to EEPROM
	passcode = passcodeTemp;

	return rtv;
}
