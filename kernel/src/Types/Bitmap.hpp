#pragma once
#include "../Types/IntDef.hpp"

class Bitmap
{
private:
public:
	size_t Size;
	uint8_t* Buffer;

	bool operator[]( uint64_t index );
	bool Set( uint64_t index, bool val );
};