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

String String::RemoveLast()
{
	String output = *this;

	if ( Length() > 0 )
		output[Length() - 1] = '\0';

	return output;
}

void String::Empty()
{
	this->c_str = (char*)"";
}

String String::GetWord( uint16_t index )
{
	bool onWord = false;
	uint16_t wordIndex = 0;
	uint16_t lastSpaceIndex = 0;

	for ( uint16_t i = 0; i < this->Length(); i++ )
	{
		if ( !onWord && ( *this )[i] != ' ' ) // Not on a word and not a space
		{
			onWord = true;

			continue;
		}
		else if ( onWord && ( *this )[i] == ' ' ) // On a word and on a space
		{
			if ( wordIndex == index ) // Wait until it has left that word
				return this->Splice( lastSpaceIndex + 1, i );

			wordIndex++;

			onWord = false;
			lastSpaceIndex = i;
			continue;
		}

		if ( ( *this )[i] == ' ' ) // On a space
			lastSpaceIndex = i;
	}

	return ( *this );
}

String String::GetAfterWord( uint16_t index )
{
	bool onWord = false;
	uint16_t wordIndex = 0;
	uint16_t lastSpaceIndex = 0;

	for ( uint16_t i = 0; i < this->Length(); i++ )
	{
		if ( !onWord && ( *this )[i] != ' ' ) // Not on a word and not a space
		{
			onWord = true;

			if ( wordIndex == index + 1 ) // Reached one word past, then splice it
				return this->Splice( lastSpaceIndex + 1, this->Length() );

			continue;
		}
		else if ( onWord && ( *this )[i] == ' ' ) // On a word and on a space
		{
			wordIndex++;

			onWord = false;
			lastSpaceIndex = i;
			continue;
		}

		if ( ( *this )[i] == ' ' ) // On a space
			lastSpaceIndex = i;
	}

	return ( *this );
}

String String::Splice( uint16_t start, uint16_t end )
{
	String output = "";

	for ( uint16_t i = start; i < end; i++ )
		output += c_str[i];

	output[end - start] = '\0';

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

// Assignment

void String::operator=( const char* other )
{
	this->c_str = (char*)other;
}

void String::operator=( String other )
{
	this->c_str = (char*)other.GetCstr();
}

char& String::operator[]( uint16_t index )
{
	return this->c_str[index];
}

bool String::operator==( String other )
{
	if ( this->Length() != other.Length() )
		return false;

	for ( uint16_t i = 0; i < this->Length(); i++ )
	{
		if ( ( *this )[i] != other[i] )
			return false;
	}

	return true;
}

bool String::operator==( const char* other )
{
	uint16_t len = 0;
	while ( other[len] != '\0' )
		len++;

	if ( this->Length() != len ) return false;

	for ( uint16_t i = 0; i < len; i++ )
	{
		if ( ( *this )[i] != other[i] )
			return false;
	}

	return true;
}