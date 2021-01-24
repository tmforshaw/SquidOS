#include "Math.hpp"

// Min

template<typename T>
T min( T v1, T v2 )
{
	if ( v1 <= v2 )
		return v1;
	else
		return v2;
}

uint8_t min( uint8_t v1, uint8_t v2 ) { return min<uint8_t>( v1, v2 ); }
int8_t min( int8_t v1, int8_t v2 ) { return min<int8_t>( v1, v2 ); }
uint16_t min( uint16_t v1, uint16_t v2 ) { return min<uint16_t>( v1, v2 ); }
int16_t min( int16_t v1, int16_t v2 ) { return min<int16_t>( v1, v2 ); }
uint32_t min( uint32_t v1, uint32_t v2 ) { return min<uint32_t>( v1, v2 ); }
int32_t min( int32_t v1, int32_t v2 ) { return min<int32_t>( v1, v2 ); }
uint64_t min( uint64_t v1, uint64_t v2 ) { return min<uint64_t>( v1, v2 ); }
int64_t min( int64_t v1, int64_t v2 ) { return min<int64_t>( v1, v2 ); }

float min( float v1, float v2 ) { return min<float>( v1, v2 ); }
double min( double v1, double v2 ) { return min<double>( v1, v2 ); }

// Max

template<typename T>
T max( T v1, T v2 )
{
	if ( v1 >= v2 )
		return v1;
	else
		return v2;
}

uint8_t max( uint8_t v1, uint8_t v2 ) { return max<uint8_t>( v1, v2 ); }
int8_t max( int8_t v1, int8_t v2 ) { return max<int8_t>( v1, v2 ); }
uint16_t max( uint16_t v1, uint16_t v2 ) { return max<uint16_t>( v1, v2 ); }
int16_t max( int16_t v1, int16_t v2 ) { return max<int16_t>( v1, v2 ); }
uint32_t max( uint32_t v1, uint32_t v2 ) { return max<uint32_t>( v1, v2 ); }
int32_t max( int32_t v1, int32_t v2 ) { return max<int32_t>( v1, v2 ); }
uint64_t max( uint64_t v1, uint64_t v2 ) { return max<uint64_t>( v1, v2 ); }
int64_t max( int64_t v1, int64_t v2 ) { return max<int64_t>( v1, v2 ); }

float max( float v1, float v2 ) { return max<float>( v1, v2 ); }
double max( double v1, double v2 ) { return max<double>( v1, v2 ); }

// Power

template<typename T>
T pow( T val, T p )
{
	T retVal = val;
	for ( T i = 1; i < p; i++ )
		retVal *= val;

	return retVal;
}

uint8_t pow( uint8_t val, uint8_t p ) { return pow<uint8_t>( val, p ); }
int8_t pow( int8_t val, int8_t p ) { return pow<int8_t>( val, p ); }
uint16_t pow( uint16_t val, uint16_t p ) { return pow<uint16_t>( val, p ); }
int16_t pow( int16_t val, int16_t p ) { return pow<int16_t>( val, p ); }
uint32_t pow( uint32_t val, uint32_t p ) { return pow<uint32_t>( val, p ); }
int32_t pow( int32_t val, int32_t p ) { return pow<int32_t>( val, p ); }
uint64_t pow( uint64_t val, uint64_t p ) { return pow<uint64_t>( val, p ); }
int64_t pow( int64_t val, int64_t p ) { return pow<int64_t>( val, p ); }

// Sqrt

template<typename T>
T sqrt( T x )
{
	// A simplified version of the Babylonion method
	short i;
	float n;

	n = x;
	i = ( 1 << 29 ) + ( i >> 1 ) - ( 1 << 22 );

	n = n + x / n;
	n = 0.25f * n + x / n;

	return n;
}

float sqrt( float x ) { return sqrt<float>( x ); }
double sqrt( double x ) { return sqrt<double>( x ); }

// Factorial

template<typename T>
T factorial( T x )
{
	if ( x < 2 )
		return 1;

	T value = 2;
	for ( short i = 3; i <= x; i++ )
		value *= i;

	return value;
}

uint64_t factorial( uint8_t x ) { return factorial<uint8_t>( x ); }
uint64_t factorial( int8_t x ) { return factorial<int8_t>( x ); }
uint64_t factorial( uint16_t x ) { return factorial<uint16_t>( x ); }
uint64_t factorial( int16_t x ) { return factorial<int16_t>( x ); }
uint64_t factorial( uint32_t x ) { return factorial<uint32_t>( x ); }
uint64_t factorial( int32_t x ) { return factorial<int32_t>( x ); }
uint64_t factorial( uint64_t x ) { return factorial<uint64_t>( x ); }
uint64_t factorial( int64_t x ) { return factorial<int64_t>( x ); }