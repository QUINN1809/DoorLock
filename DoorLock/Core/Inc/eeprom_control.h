/*
 * eeprom_control.h
 * Author: Quinn Ivison
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define EEPROM_TIMEOUT 		10000

uint16_t readLockState(void);
uint16_t readPasscode(void);
void writeEEPROM(char *msg);
void readEEPROM(char *msg);
