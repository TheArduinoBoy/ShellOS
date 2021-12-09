#pragma once

#include "stddef.h"
#include "stdint.h"

#include "drivers/io.h"
#include "graphics/print.h"

struct IDT64 {
	uint16_t offset_low;
	uint16_t selector;
	uint8_t ist;
	uint8_t types_attr;
	uint16_t offset_mid;
	uint32_t offset_high;
	uint32_t zero;
};

extern IDT64 _idt[256];
extern uint64_t isr1;
extern "C" void LoadIDT();

void InitializeIDT();