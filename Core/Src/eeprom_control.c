/*
 * eeprom_control.c
 * Author: Quinn Ivison
 */

#include "stm32f3xx_hal.h"
#include "eeprom_control.h"

extern I2C_HandleTypeDef hi2c1;
extern void print(const char *str, uint16_t size);



/*
 * readLockStateEEPROM():
 * Reads status of lock state/motor position from EEPROM
 * Inputs:
 *      (None)
 * Outputs:
 *      (lock_state) lock state value
 */
HAL_StatusTypeDef readLockStateEEPROM(bool* lock_state)
{
    return readEEPROM((uint8_t*) lock_state, 1, LOCK_STATE_ADDRESS);
}


/*
 * writeLockStateEEPROM():
 * Writes status of lock state/motor position to EEPROM
 * Inputs:
 *      (lock_state) lock state value
 * Outputs:
 *      (None)
 */
HAL_StatusTypeDef writeLockStateEEPROM(bool* lock_state)
{
    return writeEEPROM((uint8_t*) lock_state, 1, LOCK_STATE_ADDRESS);
}


/*
 * readPasscodeEEPROM():
 * Reads passcode from EEPROM
 * Inputs:
 *      (None)
 * Outputs:
 *      (passcode) buffer containing passcode
 */
HAL_StatusTypeDef readPasscodeEEPROM(uint8_t* passcode)
{
	return readEEPROM(passcode, PASSCODE_SIZE, PASSWORD_ADDRESS);
}


/*
 * writePasscodeEEPROM():
 * Writes passcode to EEPROM
 * Inputs:
 *      (passcode) buffer containing passcode
 * Outputs:
 *      (None)
 */
HAL_StatusTypeDef writePasscodeEEPROM(uint8_t* passcode)
{
    return writeEEPROM(passcode, PASSCODE_SIZE, PASSWORD_ADDRESS);
}


/*
 * writeEEPROM():
 * Writes to 24LC256 EEPROM
 * Inputs:
 *      (msg) data buffer to write
 *      (size) size of buffer
 *      (address) head of EEPROM memory to write to
 * Outputs:
 *      (None)
 */
HAL_StatusTypeDef writeEEPROM(uint8_t * msg, uint16_t size, uint16_t address)
{
	HAL_StatusTypeDef status;

	uint8_t buffer[NUM_ADDRESS_BYTES + size];
	buffer[0] = (address >> 8) & 0xff;
	buffer[1] = address & 0xff;
	memcpy(&buffer[2], msg, size);

	// Send write command
	status = HAL_I2C_Master_Transmit(&hi2c1, CONTROL_BYTE_WRITE, buffer, sizeof(buffer), EEPROM_TRANSFER_TIMEOUT);
    if(status == HAL_ERROR)
    {
        char errorWrite[] = "ERROR: Writing to EEPROM failed";
        print(errorWrite, sizeof(errorWrite));
        return status;
    }

    // Wait until EEPROM is done writing data
    status = HAL_I2C_IsDeviceReady(&hi2c1, CONTROL_BYTE_WRITE, 1, EEPROM_WAIT_TIMEOUT);
    while(status != HAL_OK) {status = HAL_I2C_IsDeviceReady(&hi2c1, CONTROL_BYTE_WRITE, 1, EEPROM_WAIT_TIMEOUT);}
    if(status == HAL_ERROR)
    {
        char errorWrite[] = "ERROR: EEPROM failed to acknowledge read initiation";
        print(errorWrite, sizeof(errorWrite));
        return status;
    }

    return status;
}


/*
 * readEEPROM():
 * Reads from 24LC256 EEPROM
 * Inputs:
 *      (size) number of bytes to read from EEPROM
 *      (address) head of EEPROM memory to read from
 * Outputs:
 *      (msg) buffer containing data read from EEPROM
 */
HAL_StatusTypeDef readEEPROM(uint8_t * msg, uint16_t size, uint16_t address)
{
	HAL_StatusTypeDef status;
    uint8_t buffer[NUM_ADDRESS_BYTES] = {(address >> 8) & 0xff, address & 0xff};

    // Send write command to select address
    status = HAL_I2C_Master_Transmit(&hi2c1, CONTROL_BYTE_WRITE, buffer, sizeof(buffer), EEPROM_TRANSFER_TIMEOUT);
    if(status == HAL_ERROR)
    {
        char errorWrite[] = "ERROR: Initiating read from EEPROM failed";
        print(errorWrite, sizeof(errorWrite));
        return status;
    }

    // Wait until EEPROM is done selecting address
    status = HAL_I2C_IsDeviceReady(&hi2c1, CONTROL_BYTE_WRITE, 1, EEPROM_WAIT_TIMEOUT);
    while(status != HAL_OK) {status = HAL_I2C_IsDeviceReady(&hi2c1, CONTROL_BYTE_WRITE, 1, EEPROM_WAIT_TIMEOUT);}
    if(status == HAL_ERROR)
    {
        char errorWrite[] = "ERROR: EEPROM failed to acknowledge read initiation";
        print(errorWrite, sizeof(errorWrite));
        return status;
    }

    // Send read command to grab data from address
	status = HAL_I2C_Master_Receive(&hi2c1, CONTROL_BYTE_READ, msg, size, EEPROM_TRANSFER_TIMEOUT);
    if(status == HAL_ERROR)
    {
        char errorWrite[] = "ERROR: Reading from EEPROM failed";
        print(errorWrite, sizeof(errorWrite));
        return status;
    }

    return status;
}
