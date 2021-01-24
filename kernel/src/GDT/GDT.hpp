#pragma once
#include <stdint.h>

struct GDT_Descriptor
{
	uint16_t Size;
	uint64_t Offset;
} __attribute__( ( packed ) );

struct GDT_Entry
{
	uint16_t Limit0; // Unused in 64-bit Long mode
	uint16_t Base0;	 // Unused in 64-bit Long mode
	uint8_t Base1;	 // Unused in 64-bit Long mode
	uint8_t AccessByte;
	uint8_t Limit1_Flags; // Lo bits are unused in 64-bit Long mode
	uint8_t Base2;		  // Unused in 64-bit Long mode
} __attribute__( ( packed ) );

struct GDT
{
	GDT_Entry Null;		  // 0x00
	GDT_Entry KernelCode; // 0x08
	GDT_Entry KernelData; // 0x10
	GDT_Entry UserNull;	  // 0x18
	GDT_Entry UserCode;	  // 0x20
	GDT_Entry UserData;	  // 0x28
} __attribute__( ( packed ) )
__attribute__( ( aligned( 0x1000 ) ) );

extern GDT DefaultGDT;

extern "C" void LoadGDT( GDT_Descriptor* gdtDescriptor );