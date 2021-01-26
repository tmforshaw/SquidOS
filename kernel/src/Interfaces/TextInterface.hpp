#pragma once
#include "../Rendering/BasicRenderer.hpp"
#include "../Rendering/Colours.hpp"
#include "../Types/IntDef.hpp"
#include "../Types/PSF1Font.hpp"

class TextUI
{
private:
	uint16_t X;
	uint16_t Y;
	uint16_t Width;
	uint16_t Height;
	PSF1_FONT* PSF1_Font;
	uint32_t BG_Col;
	uint32_t TextCol;

public:
	TextUI( uint16_t x, uint16_t y, uint16_t width, uint16_t height, PSF1_FONT* p_PSF1_Font, uint32_t p_BG_Col = GREY, uint32_t p_TextCol = GREEN );

	void Display();
};