#include "IDT.hpp"

void IDT_DescEntry::SetOffset( uint64_t offset )
{
	Offset0 = ( uint16_t )( offset & 0x000000000000FFFF );
	Offset1 = ( uint16_t )( ( offset & 0x00000000FFFF0000 ) >> 16 );
	Offset2 = ( uint32_t )( ( offset & 0xFFFFFFFF00000000 ) >> 32 );
}

uint64_t IDT_DescEntry::GetOffset()
{
	uint64_t offset = 0;

	offset |= (uint64_t)Offset0;
	offset |= ( uint64_t )( Offset1 << 16 );
	offset |= ( uint64_t )( Offset2 << 32 );

	return offset;
}