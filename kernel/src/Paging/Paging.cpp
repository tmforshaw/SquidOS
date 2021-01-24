#include "Paging.hpp"

void PageDirectoryEntry::SetFlag( PT_Flag flag, bool enabled )
{
	uint64_t bitSelector = (uint64_t)1 << flag;

	if ( enabled )
		Value |= bitSelector;
	else
		Value &= ~bitSelector;
}

bool PageDirectoryEntry::GetFlag( PT_Flag flag )
{
	uint64_t bitSelector = (uint64_t)1 << flag;
	return ( Value & bitSelector ) > 0;
}

uint64_t PageDirectoryEntry::GetAddress()
{
	return ( Value & 0x000FFFFFFFFFF000 >> 12 );
}

void PageDirectoryEntry::SetAddress( uint64_t address )
{
	address &= 0x000000FFFFFFFFFF;
	Value &= 0xFFF0000000000FFF;
	Value |= ( address ) >> 12;
}