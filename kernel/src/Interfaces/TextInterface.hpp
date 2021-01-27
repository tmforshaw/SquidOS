#pragma once
#include "../Math/Math.hpp"
#include "../Rendering/Colours.hpp"
#include "../Types/IntDef.hpp"
#include "../Types/PSF1Font.hpp"
#include "../Types/String.hpp"

class TextUI
{
private:
	Point Pos;
	uint16_t Width;
	uint16_t Height;
	uint16_t PadLeft, PadRight, PadTop, PadBottom;
	uint16_t CentraliserX, CentraliserY;

	String text;

	Point localToAbsolute( Point position );

public:
	Point AbsoluteCursorPosition;
	bool isCommandLine;

	uint32_t BG_Col;
	uint32_t TextCol;

	TextUI();
	TextUI( Point pos, uint16_t width, uint16_t height, uint32_t p_BG_Col = GREY, uint32_t p_TextCol = GREEN );

	void Display();

	uint32_t MinX();
	uint32_t MinY();

	uint32_t MaxX();
	uint32_t MaxY();

	uint32_t ColNum();
	uint32_t RowNum();

	uint32_t GetCol();
	uint32_t GetRow();

	// // Cursor Movement

	// void CurLeft();
	// void CurRight();
	// void CurUp();
	// void CurDown();

	void ClearLine( uint16_t row );
	void ClearLine();

	// Typing

	void SendChar( char chr );
	void SendBackspace();
	void SendEnter();
};

extern TextUI* SelectedTextUI;