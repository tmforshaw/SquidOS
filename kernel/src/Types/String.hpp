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

	String RemoveLast();
	void Empty();
	String Split( uint16_t start, uint16_t end );

	// Concatenation
	void operator+=( String other );
	String operator+( const String& other );
	void operator+=( char chr );
	String operator+( const char& chr );

	// Assignment
	void operator=( const char* other );
	void operator=( String other );
	char& operator[]( uint16_t i );
};