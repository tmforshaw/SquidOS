#include "C_String.hpp"

char uintTo_StringOutput[128];
const char* to_string( uint64_t value )
{
	uint8_t size; // Num of chars
	uint64_t sizeTest = value;

	while ( sizeTest / 10 > 0 ) // Divide by 10 because base 10
	{
		sizeTest /= 10;
		size++;
	}

	uint8_t index = 0; // Buffer index
	while ( value / 10 > 0 )
	{
		uint8_t remainder = value % 10;
		value /= 10;
		uintTo_StringOutput[size - index] = remainder + '0';
		index++;
	}

	uintTo_StringOutput[size - index] = value + '0';
	uintTo_StringOutput[size + 1] = '\0'; // Null terminate

	return uintTo_StringOutput;
}

char intTo_StringOutput[128];
const char* to_string( int64_t value )
{
	bool isNegative = false;

	if ( value < 0 )
	{
		isNegative = true;
		value *= -1;
		intTo_StringOutput[0] = '-';
	}

	uint8_t size; // Num of chars
	uint64_t sizeTest = value;

	while ( sizeTest / 10 > 0 ) // Divide by 10 because base 10
	{
		sizeTest /= 10;
		size++;
	}

	uint8_t index = 0; // Buffer index
	while ( value / 10 > 0 )
	{
		uint8_t remainder = value % 10;
		value /= 10;
		intTo_StringOutput[isNegative + size - index] = remainder + '0';
		index++;
	}

	intTo_StringOutput[isNegative + size - index] = value + '0';
	intTo_StringOutput[isNegative + size + 1] = '\0'; // Null terminate

	return intTo_StringOutput;
}

char hexTo_StringOutput[128];

template<typename T>
const char* to_hstring( T value, uint8_t byteNum )
{
	T* valPtr = &value;
	uint8_t* ptr;
	uint8_t temp;
	uint8_t size = byteNum * 2 - 1;

	for ( uint8_t i = 0; i < size; i++ )
	{
		ptr = ( (uint8_t*)valPtr + i );
		temp = ( ( *ptr & 0xF0 ) >> 4 );
		hexTo_StringOutput[size - ( i * 2 + 1 )] = temp + ( temp > 9 ? ( 'A' - 10 ) : '0' );
		temp = ( *ptr & 0x0F );
		hexTo_StringOutput[size - ( i * 2 + 0 )] = temp + ( temp > 9 ? ( 'A' - 10 ) : '0' );
	}

	hexTo_StringOutput[size + 1] = '\0';

	return hexTo_StringOutput;
}

const char* to_hstring( uint64_t value ) { return to_hstring<uint64_t>( value, 8 ); }
const char* to_hstring( uint32_t value ) { return to_hstring<uint32_t>( value, 4 ); }
const char* to_hstring( uint16_t value ) { return to_hstring<uint16_t>( value, 2 ); }
const char* to_hstring( uint8_t value ) { return to_hstring<uint8_t>( value, 1 ); }

char doubleTo_StringOutput[128];
const char* to_string( double value, uint8_t decimalPlaces )
{
	if ( decimalPlaces > 20 ) decimalPlaces = 20; // Limit decimal places

	char* intPtr = (char*)to_string( (int64_t)value );
	char* doublePtr = doubleTo_StringOutput;

	if ( value < 0 )
		value *= -1;

	while ( *intPtr != '\0' )
	{
		*doublePtr = *intPtr;
		intPtr++;
		doublePtr++;
	}

	*doublePtr = '.';
	doublePtr++;

	double newValue = value - (int)value;

	for ( uint8_t i = 0; i < decimalPlaces; i++ )
	{
		newValue *= 10;
		*doublePtr = (int)newValue + '0';
		newValue -= (int)newValue;
		doublePtr++;
	}

	*doublePtr = '\0';

	return doubleTo_StringOutput;
}