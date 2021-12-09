#include "print.h"

uint16_t get_cursor_position(void) {
    uint16_t pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint16_t)inb(0x3D5)) << 8;
    return pos;
}

void setCusorPosition(uint8_t x, uint8_t y) {
    uint16_t position = y * VGA_WIDTH + x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(position & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((position >> 8) & 0xFF));
}

void printString(const char* string) {
    uint8_t* chr = (uint8_t*) string;
    uint16_t index = get_cursor_position();
    while(*chr != 0) {
        switch(*chr) {
            case 10:
                index += VGA_WIDTH;
                break;
            case 13:
                index -= index % VGA_WIDTH;
                break;
            default:
                *(VGA_MEMORY + index * 2) = *chr;
                *(VGA_MEMORY + index * 2 + 1) = 0x1F;
                index++;
        }
        chr++;
    }
    setCusorPosition(index % VGA_WIDTH, index / VGA_WIDTH);
}

char hexToStringOutput[128];
template<typename T>
const char* HexToString(T value){
  T* valPtr = &value;
  uint8_t* ptr;
  uint8_t temp;
  uint8_t size = (sizeof(T)) * 2 - 1;
  uint8_t i;
  for (i = 0; i < size; i++){
    ptr = ((uint8_t*)valPtr + i);
    temp = ((*ptr & 0xF0) >> 4);
    hexToStringOutput[size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : 48);
    temp = ((*ptr & 0x0F));
    hexToStringOutput[size - (i * 2 + 0)] = temp + (temp > 9 ? 55 : 48);
  }
  hexToStringOutput[size + 1] = 0;
  return hexToStringOutput;
}

const char* HexToString(uint8_t value) { return HexToString<uint8_t>(value); }
const char* HexToString(uint16_t value) { return HexToString<uint16_t>(value); }
const char* HexToString(uint32_t value) { return HexToString<uint32_t>(value); }
const char* HexToString(uint64_t value) { return HexToString<uint64_t>(value); }
const char* HexToString(char value) { return HexToString<char>(value); }
const char* HexToString(short value) { return HexToString<short>(value); }
const char* HexToString(int value) { return HexToString<int>(value); }
const char* HexToString(long long value) { return HexToString<long long>(value); }

char integerToStringOutput[128];
template<typename T>
const char* IntegerToString(T value) {

	uint8_t isNegative = 0;

	if (value < 0) {
		isNegative = 1;
		value *= -1;
		integerToStringOutput[0] = '-';
	}

	uint8_t size = 0;
	uint64_t sizeTester = (uint64_t)value;
	while (sizeTester / 10 > 0) {
		sizeTester /= 10;
		size++;
	}

	uint8_t index = 0;
	uint64_t newValue = (uint64_t)value;
	while (newValue / 10 > 0) {
		uint8_t remainder = newValue % 10;
		newValue /= 10;
		integerToStringOutput[isNegative + size - index] = remainder + 48; 
		index++;
	}
	uint8_t remainder = newValue % 10;
	integerToStringOutput[isNegative + size - index] = remainder + 48;
	integerToStringOutput[isNegative + size + 1] = 0;
	return integerToStringOutput;
}

const char* IntegerToString(uint8_t value) { return IntegerToString<uint8_t>(value); }
const char* IntegerToString(uint16_t value) { return IntegerToString<uint16_t>(value); }
const char* IntegerToString(uint32_t value) { return IntegerToString<uint32_t>(value); }
const char* IntegerToString(uint64_t value) { return IntegerToString<uint64_t>(value); }
const char* IntegerToString(char value) { return IntegerToString<char>(value); }
const char* IntegerToString(short value) { return IntegerToString<short>(value); }
const char* IntegerToString(int value) { return IntegerToString<int>(value); }
const char* IntegerToString(long long value) { return IntegerToString<long long>(value); }

char floatToStringOutput[128];
const char* FloatToString(float value, uint8_t decimalPlaces) {
	char* intPtr = (char*)IntegerToString((int)value);
	char* floatPtr = floatToStringOutput;

	if (value < 0) {
		value *= -1;
	}

	while (*intPtr != 0) {
		*floatPtr = *intPtr;
		intPtr++;
		floatPtr++;
	}
	*floatPtr = '.';
	floatPtr++;

	float newValue = value - (int)value;

	for (uint8_t i = 0; i < decimalPlaces; i++) {
		newValue *= 10;
		*floatPtr = (int)newValue + 48;
		newValue -= (int)newValue;
		floatPtr++;
	}

	*floatPtr = 0;

	return floatToStringOutput;
}