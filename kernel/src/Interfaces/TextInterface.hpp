#pragma once
#include "../Math/Math.hpp"
#include "../Rendering/Colours.hpp"
#include "../Types/IntDef.hpp"
#include "../Types/PSF1Font.hpp"
#include "../Types/String.hpp"

class TextUI
{
private:
	PointU Pos;
	uint16_t Width;
	uint16_t Height;
	uint16_t PadLeft, PadRight, PadTop, PadBottom;
	uint16_t CentraliserX, CentraliserY;

	PointU localToAbsolute( PointU position );

public:
	PointU AbsoluteCursorPosition;
	bool isCommandLine;

	uint32_t BG_Col;
	uint32_t TextCol;
	uint32_t CursorCol;
	uint8_t CursorWidth;
	int8_t CursorOffset;

	String text;

	TextUI();
	TextUI( PointU pos, uint16_t width, uint16_t height, uint32_t p_BG_Col = GREY, uint32_t p_TextCol = GREEN );

	void DisplayCursor();
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