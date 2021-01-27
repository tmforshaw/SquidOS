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

String String::Split( uint16_t start, uint16_t end )
{
	String output = "";

	for ( uint16_t i = start; i <= end; i++ )
		output += c_str[i];

	return output;
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

void String::operator+=( char chr )
{
	uint16_t len = this->Length();

	this->c_str[len] = chr;
	this->c_str[len + 1] = '\0';
}

String String::operator+( const char& chr )
{
	String output = *this;
	output += chr;
	return output;
}