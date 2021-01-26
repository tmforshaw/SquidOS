#pragma once
#include "../Types/IntDef.hpp"

#define BLACK  0xFF000000
#define WHITE  0xFFFFFFFF
#define RED	   0xFFFF0000
#define GREEN  0xFF00FF00
#define BLUE   0xFF0000FF
#define CYAN   0xFF00FFFF
#define PINK   0xFFFF00FF
#define YELLOW 0xFFFFFF00

#define GREY 0xFF414141

#define DEFAULT_COLOUR GREEN

uint32_t RGBA_ToHex( uint8_t r, uint8_t g, uint8_t b, uint8_t a );
