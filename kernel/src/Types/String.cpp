#include "String.hpp"

String::String()
{
	c_str = "";
}

String::String( const char* str )
{
	c_str = str;
}

const char* String::GetCstr()
{
	return this->c_str;
}

uint16_t String::Length()
{
	char* chr = (char*)c_str;
	uint16_t i = 0;

	while ( *chr != '\0' )
	{
		i++;
		chr++;
	}

	return i;
}

// void String::operator+=( const String& other )
// {
// 	const char* otherC_str = other.GetCstr();
// 	uint16_t len = this->Length();
// 	uint16_t otherLen = other.Length();

// 	for ( uint16_t i = len; i < len + otherLen; i++ )
// 		this->c_str[i] = otherC_str[i - len];

// 	this->c_str[len + otherLen] = '\0';

// 	// #TODO# Memory allocation functions
// }

// String String::operator+( const String& other )
// {
// 	String output = *this;
// 	output += other;
// 	return output;
// }
