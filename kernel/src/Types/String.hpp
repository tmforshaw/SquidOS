#pragma once
#include <stdint.h>

class String
{
private:
	const char* c_str;

public:
	String();
	String( const char* str );

	const char* GetCstr();
	uint16_t Length();

	// // Concatenation
	// void operator+=( const String& other );
	// String operator+( const String& other );
};