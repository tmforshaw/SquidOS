#include "Keyboard.hpp"

#include "../Interfaces/CommandManager.hpp"
#include "../Interfaces/TextInterface.hpp"

static bool isLeftShiftPressed;
static bool isRightShiftPressed;
static bool isCapsLockOn;

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

	case CapsLock:
		isCapsLockOn = !isCapsLockOn;
		return;

	case F1:
		GlobalRenderer->PushColour( PINK );
		GlobalRenderer->Print( GlobalCommand.GetTextUI().text );
		GlobalRenderer->Endl();
		GlobalRenderer->PopColour();

	default:
		break;
	}

	char ascii = QWERTY_Keyboard::Translate( scancode, isLeftShiftPressed | isRightShiftPressed | isCapsLockOn );

	if ( ascii != 0 )
		SelectedTextUI->SendChar( ascii );
}