#pragma once

#include "stddef.h"
#include "stdint.h"

#include "drivers/io.h"

#define VGA_MEMORY (uint8_t*)0xb8000
#define VGA_WIDTH 80

void setCusorPosition(uint8_t x, uint8_t y);
void printString(const char* string);
void printChar(char chr);

const char* FloatToString(float value, uint8_t decimalPlaces);

const char* HexToString(uint8_t value);
const char* HexToString(uint16_t value);
const char* HexToString(uint32_t value);
const char* HexToString(uint64_t value);
const char* HexToString(char value);
const char* HexToString(short value);
const char* HexToString(int value);
const char* HexToString(long long value);

const char* IntegerToString(uint8_t value);
const char* IntegerToString(uint16_t value);
const char* IntegerToString(uint32_t value);
const char* IntegerToString(uint64_t value);
const char* IntegerToString(char value);
const char* IntegerToString(short value);
const char* IntegerToString(int value);
const char* IntegerToString(long long value);