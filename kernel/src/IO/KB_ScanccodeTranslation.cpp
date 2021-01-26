#include "KB_ScancodeTranslation.hpp"

namespace QWERTY_Keyboard
{
	const char ASCII_Table[] = {
		0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 0,
		'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0, 0,
		'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
		'#', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
		'*', 0, ' '
	};

	char Translate( uint8_t scancode, bool uppercase )
	{
		if ( scancode > sizeof( ASCII_Table ) / sizeof( char ) ) return 0;

		if ( uppercase )
			return ASCII_Table[scancode] - 32; // Minus offset between upper and lower case

		return ASCII_Table[scancode];
	}

} // namespace QWERTY_Keyboard