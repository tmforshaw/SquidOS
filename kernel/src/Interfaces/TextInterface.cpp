#include "TextInterface.hpp"

#include "../Rendering/BasicRenderer.hpp"

TextUI* SelectedTextUI;

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

	AbsoluteCursorPosition = localToAbsolute( { ( uint16_t )( PadLeft + CentraliserX ), ( uint16_t )( PadTop + CentraliserY ) } );

	BG_Col = p_BG_Col;
	TextCol = p_TextCol;
}

Point TextUI::localToAbsolute( Point position )
{
	return { position.X + Pos.X, position.Y + Pos.Y };
}

void TextUI::Display()
{
	GlobalRenderer->PushColour( BG_Col );
	GlobalRenderer->Rect( Pos, Width, Height, true );
	GlobalRenderer->PopColour();
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

// Cursor Movement

void TextUI::CurLeft()
{
}

void TextUI::CurRight() {}

void TextUI::CurUp() {}

void TextUI::CurDown()
{
}

// Typing

void TextUI::SendChar( char chr )
{
	if ( GetCol() >= ColNum() - 1 ) // On the final column
	{
		if ( GetRow() < RowNum() - 1 ) // There are lines available
		{
			GlobalRenderer->PutChar( chr, SelectedTextUI->AbsoluteCursorPosition.X, SelectedTextUI->AbsoluteCursorPosition.Y );

			// New line
			AbsoluteCursorPosition.X = MinX();
			AbsoluteCursorPosition.Y += GlobalRenderer->PSF1_Font->psf1_Header->charsize;
		}
		else // Fill in the last character
		{
			if ( GetCol() == ColNum() - 1 )
			{
				GlobalRenderer->PutChar( chr, SelectedTextUI->AbsoluteCursorPosition.X, SelectedTextUI->AbsoluteCursorPosition.Y );
				SelectedTextUI->AbsoluteCursorPosition.X += PSF1_FontWidth;
			}
		}
	}
	else
	{
		GlobalRenderer->PutChar( chr, SelectedTextUI->AbsoluteCursorPosition.X, SelectedTextUI->AbsoluteCursorPosition.Y );
		AbsoluteCursorPosition.X += PSF1_FontWidth;
	}
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
}

void TextUI::SendEnter()
{
	if ( GetRow() < RowNum() - 1 ) // not on final row
	{
		AbsoluteCursorPosition.Y += GlobalRenderer->PSF1_Font->psf1_Header->charsize;
		AbsoluteCursorPosition.X = MinX();
	}
}