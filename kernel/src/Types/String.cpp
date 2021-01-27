#include "String.hpp"

String::String()
{
	c_str = (char*)"";
}

String::String( const char* str )
{
	c_str = (char*)str;
}

const char* String::GetCstr()
{
	return (const char*)this->c_str;
}

uint16_t String::Length()
{
	char* chr = c_str;
	uint16_t i = 0;

	while ( *chr != '\0' )
	{
		i++;
		chr++;
	}

	return i;
}

void String::operator+=( String other )
{
	uint16_t len = this->Length();
	uint16_t otherLen = other.Length();

	for ( uint16_t i = len; i < len + otherLen; i++ )
		this->c_str[i] = other.GetCstr()[i - len];

	this->c_str[len + otherLen] = '\0';
}

String String::operator+( const String& other )
{
	String output = *this;
	output += other;
	return output;
}
