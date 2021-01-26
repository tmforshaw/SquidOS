#include "Keyboard.hpp"

#include "../Interfaces/TextInterface.hpp"

static bool isLeftShiftPressed;
static bool isRightShiftPressed;

void HandleKeyboard( uint8_t scancode )
{
	switch ( scancode )
	{
	case LeftShift:
		isLeftShiftPressed = true;
		return;

	case LeftShift + 0x80:
		isLeftShiftPressed = false;
		return;

	case RightShift:
		isRightShiftPressed = true;
		return;

	case RightShift + 0x80:
		isRightShiftPressed = false;
		return;

	case Enter:
		SelectedTextUI->SendEnter();
		return;

	case Spacebar:
		SelectedTextUI->SendChar( ' ' );
		return;

	case Backspace:
		SelectedTextUI->SendBackspace();
		return;

	default:
		break;
	}

	char ascii = QWERTY_Keyboard::Translate( scancode, isLeftShiftPressed | isRightShiftPressed );

	if ( ascii != 0 )
		SelectedTextUI->SendChar( ascii );
}