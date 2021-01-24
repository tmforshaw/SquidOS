#pragma once
#include <stdint.h>

struct Point
{
	uint32_t X;
	uint32_t Y;
};

// Min
uint8_t min( uint8_t v1, uint8_t v2 );
int8_t min( int8_t v1, int8_t v2 );
uint16_t min( uint16_t v1, uint16_t v2 );
int16_t min( int16_t v1, int16_t v2 );
uint32_t min( uint32_t v1, uint32_t v2 );
int32_t min( int32_t v1, int32_t v2 );
uint64_t min( uint64_t v1, uint64_t v2 );
int64_t min( int64_t v1, int64_t v2 );

float min( float v1, float v2 );
double min( double v1, double v2 );

// Max
uint8_t max( uint8_t v1, uint8_t v2 );
int8_t max( int8_t v1, int8_t v2 );
uint16_t max( uint16_t v1, uint16_t v2 );
int16_t max( int16_t v1, int16_t v2 );
uint32_t max( uint32_t v1, uint32_t v2 );
int32_t max( int32_t v1, int32_t v2 );
uint64_t max( uint64_t v1, uint64_t v2 );
int64_t max( int64_t v1, int64_t v2 );

float max( float v1, float v2 );
double max( double v1, double v2 );

// Power
uint8_t pow( uint8_t val, uint8_t p );
int8_t pow( int8_t val, int8_t p );
uint16_t pow( uint16_t val, uint16_t p );
int16_t pow( int16_t val, int16_t p );
uint32_t pow( uint32_t val, uint32_t p );
int32_t pow( int32_t val, int32_t p );
uint64_t pow( uint64_t val, uint64_t p );
int64_t pow( int64_t val, int64_t p );

// Sqrt
float sqrt( float x );
double sqrt( double x );

// Factorial
uint64_t factorial( uint8_t x );
uint64_t factorial( int8_t x );
uint64_t factorial( uint16_t x );
uint64_t factorial( int16_t x );
uint64_t factorial( uint32_t x );
uint64_t factorial( int32_t x );
uint64_t factorial( uint64_t x );
uint64_t factorial( int64_t x );