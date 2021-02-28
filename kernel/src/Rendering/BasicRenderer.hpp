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
	PointU CursorPosition;
	Framebuffer* TargetFramebuffer;
	PSF1_FONT* PSF1_Font;
	uint32_t Colour;
	uint32_t ClearColour;
	uint32_t ColourStack; // Used to temporarily hold colours

	uint32_t MouseCursorBuffer[16 * 16];
	uint32_t MouseCursorBufferNext[16 * 16];
	bool MouseDrawn;

	BasicRenderer( Framebuffer* p_TargetFramebuffer, PSF1_FONT* p_PSF1_Font, uint32_t p_Colour = GREEN, uint32_t p_ClearColour = BLACK );

	// Text

	void PutChar( char chr, uint32_t xOff, uint32_t yOff );
	void Print( const char* str );
	void Print( String str );
	void Print( char chr );

	void Clear( uint32_t colour = BLACK );
	void ClearChar( uint16_t xOff, uint16_t yOff, uint32_t colour = BLACK );
	void Endl( uint16_t amt = 1 );

	// Mouse

	void ClearMouseCursor( Point pos, uint8_t* MouseCursor );
	void DrawOverlayMouseCursor( Point pos, uint8_t* MouseCursor, uint32_t colour = WHITE );

	// Colours

	void PushColour( uint32_t p_Colour );
	void PopColour();

	// Drawing

	void PutPix( PointU p );
	uint32_t GetPix( PointU p );

	void Line( PointU p1, PointU p2 );

	void Rect( PointU pos, uint16_t width, uint16_t height, bool fill = true );
	void Circle( PointU pos, uint16_t r, bool fill = true );
	void Sin( PointU pos, uint16_t width, uint16_t step, uint16_t amplitude = 1, float timePeriod = 75.0f, float xOffset = 0.0f );
	void Cos( PointU pos, uint16_t width, uint16_t step, uint16_t amplitude = 1, float timePeriod = 75.0f, float xOffset = 0.0f );

private:
	void DisplayBresenham( PointU pos, int16_t x, int16_t y ); // Hide so it cannot be used outside of class
};

extern BasicRenderer* GlobalRenderer;