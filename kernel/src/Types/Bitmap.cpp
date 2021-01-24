#include "Bitmap.hpp"

bool Bitmap::operator[]( uint64_t index )
{
	if ( index > Size * 8 ) return false; // Stop overflow

	const uint64_t byteIndex = index / 8;
	const uint8_t bitIndex = index % 8;
	const uint8_t bitIndexer = 0b10000000 >> bitIndex;

	if ( ( Buffer[byteIndex] & bitIndexer ) > 0 )
		return true;

	return false;
}

bool Bitmap::Set( uint64_t index, bool val )
{
	if ( index > Size * 8 ) return false; // Stop overflow

	const uint64_t byteIndex = index / 8;
	const uint8_t bitIndex = index % 8;
	const uint8_t bitIndexer = 0b10000000 >> bitIndex;
	Buffer[byteIndex] &= ~bitIndexer; // Turns the bit off

	if ( val )
		Buffer[byteIndex] |= bitIndexer; // Turns the bit on

	return true;
}
