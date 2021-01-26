#pragma once
#include "../Types/IntDef.hpp"

const char* to_string( uint64_t value );
const char* to_string( int64_t value, bool negative = false );

const char* to_hstring( uint64_t value );
const char* to_hstring( uint32_t value );
const char* to_hstring( uint16_t value );
const char* to_hstring( uint8_t value );

const char* to_string( double value, uint8_t decimalPlaces = 3 );
