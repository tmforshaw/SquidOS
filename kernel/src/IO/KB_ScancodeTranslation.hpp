#pragma once
#include "../Types/IntDef.hpp"

namespace QWERTY_Keyboard
{

#define LeftShift  0x2A
#define RightShift 0x36
#define Enter	   0x1C
#define Backspace  0x0E
#define Spacebar   0x39

	extern const char ASCII_Table[];
	char Translate( uint8_t scancode, bool uppercase = false );
} // namespace QWERTY_Keyboard