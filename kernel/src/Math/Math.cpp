#include "Math.hpp"

// Constants

float Math::PI = 3.14159265f;
double Math::PI_Long = 3.141592653589793;

float Math::PI_Sqr = 9.86960440f;
double Math::PI_Sqr_Long = 9.86960440109; // Could be more accurate

float Math::TAU = 6.28318530f;
double Math::TAU_LONG = 6.2831853071795862;

float Math::TAU_Sqr = 39.4784176f;
double Math::TAU_Sqr_Long = 39.47841760436;

// Functions

// Abs

template<typename T>
T abs( T x )
{
	if ( x >= 0 )
		return x;

	return x * -1;
}

int8_t abs( int8_t x ) { return abs<int8_t>( x ); }
int16_t abs( int16_t x ) { return abs<int16_t>( x ); }
int32_t abs( int32_t x ) { return abs<int32_t>( x ); }
int64_t abs( int64_t x ) { return abs<int64_t>( x ); }

float abs( float x ) { return abs<float>( x ); }
double abs( double x ) { return abs<double>( x ); }

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

// Floor

template<typename T>
T floor( T x )
{
	return (float)(uint64_t)x;
}

float floor( float x ) { return floor<float>( x ); }
double floor( double x ) { return floor<double>( x ); }

// Ceil

template<typename T>
T ceil( T x )
{
	return (float)(uint64_t)x + 1;
}

float ceil( float x ) { return floor<float>( x ); }
double ceil( double x ) { return floor<double>( x ); }

// Fmod

float fmod( float val, float div )
{
	if ( div == 0 ) return 0; // #TODO# Math error

	float ratio = val / div;

	return val - div * floor( ratio );
}

// Power

template<typename T1, typename T2 = T1>
T1 pow( T1 val, T2 p )
{
	T1 retVal = val;
	for ( T2 i = 1; i < p; i++ )
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

float pow( float val, uint32_t p ) { return pow<float, uint32_t>( val, p ); }
float pow( float val, int32_t p ) { return pow<float, int32_t>( val, p ); }
double pow( double val, uint32_t p ) { return pow<double, uint32_t>( val, p ); }
double pow( double val, int32_t p ) { return pow<double, int32_t>( val, p ); }

// Sqrt

template<typename T>
T sqrt( T x )
{
	if ( x < 0 ) return 0; // #TODO# FIX THIS IN FUTURE (Add Math Error)
	if ( x == 0 ) return 0;

	T xhi = x;
	T xlo = 0;
	T guess = x / 2;

	while ( abs( ( guess * guess - x ) / guess ) > 0.00001 ) // Get the relative error
	{
		// // Getting the mean
		// if ( guess * guess > x )
		// 	xhi = guess;
		// else
		// 	xlo = guess;

		// guess = ( xhi + xlo ) / 2; // Get the mean

		// if ( guess == xhi || guess == xlo ) break; // Exit incase of infinite loops

		// Newton's Method
		guess -= ( guess * guess - x ) / ( 2 * guess );
	}

	return guess;
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

// Radians and Degrees

float radiansToDegrees( float rad ) { return rad * 180 / Math::PI; }
double radiansToDegrees( double rad ) { return rad * 180 / Math::PI_Long; }

float degreesToRadians( float deg ) { return deg * Math::PI / 180; }
double degreesToRadians( double deg ) { return deg * Math::PI_Long / 180; }

// Trig

template<typename T>
T sin( T x )
{
	T value = fmod( x, Math::PI ); // Clamp between 0 and pi
	T retValue;

	// // Using a quadratic graph
	// retValue = -0.417698 * value * value + 1.312236 * value - 0.050465;

	// Using more accurate function

	retValue = ( 16.0 * value * ( Math::PI - value ) ) / ( 5.0 * Math::PI_Sqr - 4.0 * value * ( Math::PI - value ) );

	if ( fmod( x, 2 * Math::TAU ) > Math::PI ) // Adjust x to be between 0 and 2PI and return correct value
	{
		return -retValue;
	}

	return retValue;
}

float sin( float x ) { return sin<float>( x ); }
double sin( double x ) { return sin<double>( x ); }

float cos( float x ) { return sin<float>( Math::PI / 2 - x ); }
double cos( double x ) { return sin<double>( Math::PI_Long / 2 - x ); }

float tan( float x ) { return sin( x ) / cos( x ); }
double tan( double x ) { return sin( x ) / cos( x ); }

// Inverse trig

template<typename T>
T asin( T x )
{
	T aX = fmod( x + 1, 2 ) - 1; // Clamps between 1 and -1
	T value = aX;
	T retValue;
	bool isNegative = ( aX < 0 );
	value = abs( value );

	// Approximation using Bhaskara I

	retValue = ( Math::TAU / 4 ) * ( 1 - 2 * sqrt( ( 1 - value ) / ( 4 + value ) ) );

	return value;
}

float asin( float x ) { return asin<float>( x ); }
double asin( double x ) { return asin<double>( x ); }

float acos( float x ) { return asin<float>( Math::PI / 2 - x ); }
double acos( double x ) { return asin<double>( Math::PI_Long / 2 - x ); }

#define A	   0.0776509570923569
#define B	   -0.287434475393028
#define PI_D_4 ( Math::PI / 4 )
#define C	   ( PI_D_4 - A - B )

template<typename T>
T atan( T x )
{
	T aX = fmod( x + 1, 2 ) - 1; // Clamps between 1 and -1

	double xx = x * x;
	return ( ( A * xx + B ) * xx + C ) * x;
}

float atan( float x ) { return atan<float>( x ); }
double atan( double x ) { return atan<double>( x ); }

template<typename T>
T atan2( T dy, T dx )
{
	if ( dx == 0 )
	{
		return ( dy < 0 ) ? Math::PI / 2 : -Math::PI / 2; // #TODO# Should return error if dy == 0
	}

	return atan( dy / dx );
}

float atan2( float dx, float dy ) { return atan2<float>( dx, dy ); }
double atan2( double dx, double dy ) { return atan2<double>( dx, dy ); }