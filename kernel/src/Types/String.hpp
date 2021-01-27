#pragma once
#include "../Types/IntDef.hpp"

class String
{
private:
	char* c_str;

public:
	String();
	String( const char* str );

	const char* GetCstr();
	uint16_t Length();

	// Concatenation
	void operator+=( String other );
	String operator+( const String& other );
};