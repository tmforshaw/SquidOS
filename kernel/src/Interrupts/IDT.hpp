#pragma once
#include <stdint.h>

#define IDT_TA_InterruptGate 0b10001110
#define IDT_TA_CallGate		 0b10001100
#define IDT_TA_TrapGate		 0b10001111

struct IDT_DescEntry
{
	uint16_t Offset0;
	uint16_t Selector;
	uint8_t Ist; // Interrupt stack table offset
	uint8_t Type_attr;
	uint16_t Offset1;
	uint32_t Offset2;
	uint32_t Ignore;

	void SetOffset( uint64_t offset );
	uint64_t GetOffset();
};

struct IDTR
{
	uint16_t Limit;
	uint64_t Offset;
} __attribute__( ( packed ) );