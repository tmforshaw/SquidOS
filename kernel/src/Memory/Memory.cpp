#include "Memory.hpp"

#include "../Paging/PageFrameAllocator.hpp"
#include "../Rendering/BasicRenderer.hpp"
#include "../Types/C_String.hpp"

static uint64_t memorySizeBytes;
uint64_t GetMemorySize( EFI_MEMORY_DESCRIPTOR* m_Map, uint64_t m_MapEntries, uint64_t m_MapDescriptorSize )
{
	if ( memorySizeBytes > 0 ) return memorySizeBytes; // Has already been calculated

	for ( int i = 0; i < m_MapEntries; i++ )
	{
		EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)( (uint64_t)m_Map + ( i * m_MapDescriptorSize ) );
		memorySizeBytes += desc->numPages * 4096;
	}

	return memorySizeBytes;
}

void memset( void* start, uint8_t value, uint64_t byteAmt )
{
	for ( uint64_t i = 0; i < byteAmt; i++ )
		*(uint8_t*)( (uint64_t)start + i ) = value;
}

void PrintMemoryDebug()
{
	GlobalRenderer->Print( "Total RAM: " );
	GlobalRenderer->Colour = PINK;
	GlobalRenderer->Print( to_string( memorySizeBytes / 1024 / 1024 ) );
	GlobalRenderer->Colour = DEFAULT_COLOUR;
	GlobalRenderer->Print( " MiB" );
	GlobalRenderer->Endl();

	GlobalRenderer->Print( "Free RAM: " );
	GlobalRenderer->Colour = PINK;
	GlobalRenderer->Print( to_string( GlobalAllocator.GetFreeRAM() / 1024 / 1024 ) );
	GlobalRenderer->Colour = DEFAULT_COLOUR;
	GlobalRenderer->Print( " MiB" );
	GlobalRenderer->Endl();

	GlobalRenderer->Print( "Used RAM: " );
	GlobalRenderer->Colour = PINK;
	GlobalRenderer->Print( to_string( GlobalAllocator.GetUsedRAM() / 1024 ) );
	GlobalRenderer->Colour = DEFAULT_COLOUR;
	GlobalRenderer->Print( " KiB" );
	GlobalRenderer->Endl();

	GlobalRenderer->Print( "Reserved RAM: " );
	GlobalRenderer->Colour = PINK;
	GlobalRenderer->Print( to_string( GlobalAllocator.GetReservedRAM() / 1024 / 1024 ) );
	GlobalRenderer->Colour = DEFAULT_COLOUR;
	GlobalRenderer->Print( " MiB" );
	GlobalRenderer->Endl();
}