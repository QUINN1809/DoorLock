/*
 * eeprom_control.c
 * Author: Quinn Ivison
 */

#include "stm32f3xx_hal.h"
#include "eeprom_control.h"

extern I2C_HandleTypeDef hi2c1;

extern void print(const char *str, uint16_t size);
/*
bool readLockState()
{
	uint16_t lockState = 0;
	readEEPROM(&lockState, 1);

	return (bool) lockState;
}

void readPasscode(uint16_t* passcode, uint16_t size)
{
	readEEPROM(passcode, size);
}
*/
void writeEEPROM(uint8_t * msg, uint16_t size, uint16_t address)
{
	HAL_StatusTypeDef status;
	uint8_t buffer[3] = {(address>>8) & 0xff, address & 0xff, 37};

	//status = HAL_I2C_Mem_Write(&hi2c1, CONTROL_BYTE_WRITE, address, sizeof(address), msg, size, EEPROM_TIMEOUT);
	status = HAL_I2C_Master_Transmit(&hi2c1, CONTROL_BYTE_WRITE, buffer, sizeof(buffer), EEPROM_TIMEOUT);
	char errorWrite[] = {'\n', 'W', ':', ' ', status + '0'};
	print(errorWrite, sizeof(errorWrite));
}

void readEEPROM(uint8_t * msg, uint16_t size, uint16_t address)
{
	HAL_StatusTypeDef status;

	status = HAL_I2C_Mem_Write(&hi2c1, CONTROL_BYTE_WRITE, address, sizeof(address), 0, 0, EEPROM_TIMEOUT);
	char errorWrite[] = {'\n', 'W', ':', ' ', status + '0'};
	//print(errorWrite, sizeof(errorWrite));

	HAL_Delay(5);

	status = HAL_I2C_Mem_Read(&hi2c1, CONTROL_BYTE_READ, address, sizeof(address), msg, size, EEPROM_TIMEOUT);
	char errorRead[] = {'\n', 'R', ':', ' ', status + '0'};
	//print(errorRead, sizeof(errorRead));
}
