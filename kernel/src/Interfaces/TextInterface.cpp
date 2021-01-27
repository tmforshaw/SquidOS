#include "TextInterface.hpp"

#include "../Interfaces/Commands.hpp"
#include "../Rendering/BasicRenderer.hpp"

TextUI* SelectedTextUI;

TextUI::TextUI()
{
	Pos = { 0, 0 };
	Width = 0;
	Height = 0;

	PadLeft = 2;
	PadRight = 2;
	PadTop = 2;
	PadBottom = 2;

	CentraliserX = 0;
	CentraliserY = 0;

	text = String( "" );

	AbsoluteCursorPosition = { 0, 0 };

	isCommandLine = false;

	BG_Col = DEFAULT_BG_COLOUR;
	TextCol = DEFAULT_COLOUR;
}

TextUI::TextUI( Point pos, uint16_t width, uint16_t height, uint32_t p_BG_Col, uint32_t p_TextCol )
{
	Pos = pos;
	Width = width;
	Height = height;

	PadLeft = 2;
	PadRight = 2;
	PadTop = 2;
	PadBottom = 2;

	// Calculate the values needed to centralise the text
	CentraliserX = ( ( Width - PadLeft - PadRight ) % PSF1_FontWidth ) / 2;									   // Remainder divided by two
	CentraliserY = ( ( Height - PadTop - PadBottom ) % GlobalRenderer->PSF1_Font->psf1_Header->charsize ) / 2; // Remainder divided by two

	text = String( "" );

	AbsoluteCursorPosition = localToAbsolute( { ( uint16_t )( PadLeft + CentraliserX ), ( uint16_t )( PadTop + CentraliserY ) } );

	isCommandLine = false;

	BG_Col = p_BG_Col;
	TextCol = p_TextCol;
}

Point TextUI::localToAbsolute( Point position )
{
	return { position.X + Pos.X, position.Y + Pos.Y };
}

#include "../Types/C_String.hpp"

void TextUI::Display()
{
	GlobalRenderer->PushColour( BG_Col );
	GlobalRenderer->Rect( Pos, Width, Height, true );
	GlobalRenderer->PopColour();

	if ( text.Length() == 0 ) // Return if there is no text
		return;

	uint16_t i = 0;
	uint16_t y = MinY();
	for ( uint16_t x = MinX(); x < MaxX(); x += PSF1_FontWidth )
	{
		// Special cases
		if ( i >= text.Length() )
			return; // End of text

		if ( text[i] == '\n' ) // New line
		{
			y += GlobalRenderer->PSF1_Font->psf1_Header->charsize;
			x = MinX() - PSF1_FontWidth;
		}
		else
			GlobalRenderer->PutChar( text[i], x, y );

		i++;
	}
}

static uint32_t Max_x = 0, Max_y = 0;

uint32_t TextUI::MinX()
{
	return Pos.X + PadLeft + CentraliserX;
}

uint32_t TextUI::MinY()
{
	return Pos.Y + PadTop + CentraliserY;
}

uint32_t TextUI::MaxX()
{
	if ( Max_x > 0 ) return Max_x;

	Max_x = min( GlobalRenderer->TargetFramebuffer->Width, Pos.X + Width - PadRight );
	return Max_x;
}

uint32_t TextUI::MaxY()
{
	if ( Max_y > 0 ) return Max_y;

	Max_y = min( GlobalRenderer->TargetFramebuffer->Height, Pos.Y + Height - PadBottom );
	return Max_y;
}

uint32_t TextUI::ColNum()
{
	return ( MaxX() - MinX() ) / PSF1_FontWidth;
}

uint32_t TextUI::RowNum()
{
	return ( MaxY() - MinY() ) / GlobalRenderer->PSF1_Font->psf1_Header->charsize;
}

uint32_t TextUI::GetCol()
{
	return ( AbsoluteCursorPosition.X - Pos.X ) / PSF1_FontWidth;
}

uint32_t TextUI::GetRow()
{
	return ( AbsoluteCursorPosition.Y - Pos.Y ) / GlobalRenderer->PSF1_Font->psf1_Header->charsize;
}

// // Cursor Movement

// void TextUI::CurLeft() {}

// void TextUI::CurRight() {}

// void TextUI::CurUp() {}

// void TextUI::CurDown() {}

void TextUI::ClearLine( uint16_t row )
{
	GlobalRenderer->PushColour( BG_Col );
	GlobalRenderer->Rect( { Pos.X + PadRight + CentraliserX, Pos.Y + PadTop + row * GlobalRenderer->PSF1_Font->psf1_Header->charsize }, Width - PadRight, GlobalRenderer->PSF1_Font->psf1_Header->charsize, true );
	GlobalRenderer->PopColour();
}

void TextUI::ClearLine()
{
	ClearLine( GetRow() );
}

// Typing

void TextUI::SendChar( char chr )
{
	if ( GetCol() >= ColNum() - 1 ) // On the final column
	{
		if ( GetRow() < RowNum() - 1 ) // There are lines available
		{
			// GlobalRenderer->PutChar( chr, SelectedTextUI->AbsoluteCursorPosition.X, SelectedTextUI->AbsoluteCursorPosition.Y );
			text += chr; // Add to text variable

			text += '\n'; // Add newline to text

			// New line
			AbsoluteCursorPosition.X = MinX();
			AbsoluteCursorPosition.Y += GlobalRenderer->PSF1_Font->psf1_Header->charsize;
		}
		else // Fill in the last character
		{
			if ( GetCol() == ColNum() - 1 )
			{
				// GlobalRenderer->PutChar( chr, SelectedTextUI->AbsoluteCursorPosition.X, SelectedTextUI->AbsoluteCursorPosition.Y );
				SelectedTextUI->AbsoluteCursorPosition.X += PSF1_FontWidth;
				text += chr; // Add to text variable
			}
		}
	}
	else
	{
		// GlobalRenderer->PutChar( chr, SelectedTextUI->AbsoluteCursorPosition.X, SelectedTextUI->AbsoluteCursorPosition.Y );
		AbsoluteCursorPosition.X += PSF1_FontWidth;
		text += chr; // Add to text variable
	}

	Display();
}

void TextUI::SendBackspace()
{
	if ( GetCol() == 0 ) // In first column
	{
		SelectedTextUI->AbsoluteCursorPosition.X = ( PSF1_FontWidth * ( ColNum() - 1 ) + MinX() ); // Puts it in the last column

		if ( GetRow() > 0 ) // Isn't on the first row
			AbsoluteCursorPosition.Y -= GlobalRenderer->PSF1_Font->psf1_Header->charsize;
		else
		{
			AbsoluteCursorPosition.Y = MinY();
			AbsoluteCursorPosition.X = MinX();
		}
	}
	else
		SelectedTextUI->AbsoluteCursorPosition.X -= PSF1_FontWidth;

	GlobalRenderer->ClearChar( AbsoluteCursorPosition.X, AbsoluteCursorPosition.Y, BG_Col );
	text = text.RemoveLast(); // Remove the last char of text
}

void TextUI::SendEnter()
{
	if ( isCommandLine )
	{
		GlobalCommand.SendCommand(); // Send off the command
		return;
	}

	// GlobalRenderer->Print( text );

	if ( GetRow() < RowNum() - 1 ) // not on final row
	{
		// Add the amount of spaces necessary to text
		for ( uint16_t i = 0; i < ColNum() - GetCol(); i++ )
			text += ' ';

		AbsoluteCursorPosition.Y += GlobalRenderer->PSF1_Font->psf1_Header->charsize;
		AbsoluteCursorPosition.X = MinX();

		text += '\n'; // Add newline to text
	}
}
