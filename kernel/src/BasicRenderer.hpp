#pragma once
#include "FrameBuffer.hpp"
#include "Math.hpp"
#include "PSF1Font.hpp"

class BasicRenderer
{
private:
public:
	Point CursorPosition;
	Framebuffer* TargetFramebuffer;
	PSF1_FONT* PSF1_Font;
	unsigned int Colour;

	BasicRenderer( Framebuffer* p_TargetFramebuffer, PSF1_FONT* p_PSF1_Font, unsigned int p_Colour = 0xFF00FFFF );

	void PutChar( char chr, unsigned int xOff, unsigned int yOff );
	void Print( const char* str );
};