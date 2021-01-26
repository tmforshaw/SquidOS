#pragma once
#include "../Math/Math.hpp"
#include "../Types/IntDef.hpp"
#include "../Types/PSF1Font.hpp"
#include "../Types/String.hpp"
#include "Colours.hpp"
#include "FrameBuffer.hpp"

class BasicRenderer
{
public:
	Point CursorPosition;
	Framebuffer* TargetFramebuffer;
	PSF1_FONT* PSF1_Font;
	uint32_t Colour;
	uint32_t ClearColour;

	BasicRenderer( Framebuffer* p_TargetFramebuffer, PSF1_FONT* p_PSF1_Font, uint32_t p_Colour = GREEN, uint32_t p_ClearColour = BLACK );

	void PutChar( char chr, uint32_t xOff, uint32_t yOff );
	void PutChar( char chr );
	void Print( const char* str );
	void Print( String str );

	void Clear( uint32_t colour = BLACK );
	void ClearChar( uint32_t colour = BLACK );
	void Endl( uint16_t amt = 1 );

	// Drawing

	void PutPix( Point p, uint32_t col = DEFAULT_COLOUR );

	void Line( Point p1, Point p2 );

	void Rect( Point pos, uint16_t width, uint16_t height, bool fill = true );
	void Circle( Point pos, uint16_t r, bool fill = true );

private:
	void DisplayBresenham( Point pos, int16_t x, int16_t y ); // Hide so it cannot be used outside of class
};

extern BasicRenderer* GlobalRenderer;