/*
 * eeprom_control.h
 * Author: Quinn Ivison
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define EEPROM_TIMEOUT 		10000

#define CONTROL_BYTE_READ	0b10100001
#define CONTROL_BYTE_WRITE	0b10100000

#define NUM_ADDRESS_BYTES	2

#define DEVICE_ADDRESS		0b000
#define PASSWORD_ADDRESS	0x256

uint16_t readLockState(void);
uint16_t readPasscode(void);
void writeEEPROM(uint8_t * msg, uint16_t size, uint16_t address);
void readEEPROM(uint8_t * msg, uint16_t size, uint16_t address);
