#pragma once
#include "EFI_Memory.hpp"

#include <stdint.h>

uint64_t GetMemorySize( EFI_MEMORY_DESCRIPTOR* m_Map, uint64_t m_MapEntries, uint64_t m_MapDescriptorSize );
void memset( void* start, uint8_t value, uint64_t byteAmt );