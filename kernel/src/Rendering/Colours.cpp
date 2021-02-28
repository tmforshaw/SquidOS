#include "Colours.hpp"

uint32_t RGBA_ToHex( uint8_t r, uint8_t g, uint8_t b, uint8_t a )
{
	// ARGB
	return ( (uint32_t)a << 0x18 ) + ( (uint32_t)r << 0x10 ) + ( (uint32_t)g << 0x08 ) + (uint32_t)b;
}