#include "Memory.hpp"

uint64_t GetMemorySize( EFI_MEMORY_DESCRIPTOR* m_Map, uint64_t m_MapEntries, uint64_t m_MapDescriptorSize )
{
	static uint64_t memorySizeBytes;

	if ( memorySizeBytes > 0 ) return memorySizeBytes; // Has already been calculated

	for ( int i = 0; i < m_MapEntries; i++ )
	{
		EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)( (uint64_t)m_Map + ( i * m_MapDescriptorSize ) );
		memorySizeBytes += desc->numPages * 4096;
	}

	return memorySizeBytes;
}