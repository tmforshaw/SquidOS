#pragma once
#include "../Math/Math.hpp"
#include "../Types/PSF1Font.hpp"
#include "Colours.hpp"
#include "FrameBuffer.hpp"

#include <stdint.h>

class BasicRenderer
{
private:
public:
	Point CursorPosition;
	Framebuffer* TargetFramebuffer;
	PSF1_FONT* PSF1_Font;
	uint32_t Colour;

	BasicRenderer( Framebuffer* p_TargetFramebuffer, PSF1_FONT* p_PSF1_Font, uint32_t p_Colour = GREEN );

	void PutChar( char chr, uint32_t xOff, uint32_t yOff );
	void Print( const char* str );

	void Clear( uint32_t colour );
	void Endl( uint16_t amt = 1 );

	// Drawing

	void Line( Point p1, Point p2 );

	void Rect( Point pos, uint16_t width, uint16_t height, bool fill = true );
	void Circle( Point pos, uint16_t r, bool fill = true );
};

extern BasicRenderer* GlobalRenderer;