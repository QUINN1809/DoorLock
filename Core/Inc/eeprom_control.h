/*
 * eeprom_control.h
 * Author: Quinn Ivison
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "main.h"

#define EEPROM_TRANSFER_TIMEOUT     10000
#define EEPROM_WAIT_TIMEOUT         50

#define CONTROL_BYTE_READ	        0b10100001
#define CONTROL_BYTE_WRITE	        0b10100000

#define NUM_ADDRESS_BYTES	        2

#define LOCK_STATE_ADDRESS          0x512
#define PASSWORD_ADDRESS	        0x256


HAL_StatusTypeDef readLockStateEEPROM(bool* lock_state);
HAL_StatusTypeDef writeLockStateEEPROM(bool* lock_state);

HAL_StatusTypeDef readPasscodeEEPROM(uint8_t* passcode);
HAL_StatusTypeDef writePasscodeEEPROM(uint8_t* passcode);

HAL_StatusTypeDef writeEEPROM(uint8_t * msg, uint16_t size, uint16_t address);
HAL_StatusTypeDef readEEPROM(uint8_t * msg, uint16_t size, uint16_t address);
