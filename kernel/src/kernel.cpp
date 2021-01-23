#include "BasicRenderer.hpp"
#include "C_String.hpp"

extern "C" void _start( Framebuffer* framebuffer, PSF1_FONT* psf1_font )
{
	BasicRenderer newRenderer = BasicRenderer( framebuffer, psf1_font );

	newRenderer.Print( to_string( (uint64_t)12345 ) );
	newRenderer.CursorPosition.Y += newRenderer.PSF1_Font->psf1_Header->charsize;
	newRenderer.CursorPosition.X = 0;
	newRenderer.Print( to_string( (int64_t)-12345 ) );
	newRenderer.CursorPosition.Y += newRenderer.PSF1_Font->psf1_Header->charsize;
	newRenderer.CursorPosition.X = 0;
	newRenderer.Print( to_string( (double)12.345 ) );
	newRenderer.CursorPosition.Y += newRenderer.PSF1_Font->psf1_Header->charsize;
	newRenderer.CursorPosition.X = 0;
	newRenderer.Print( to_string( (double)-12.345 ) );
	newRenderer.CursorPosition.Y += newRenderer.PSF1_Font->psf1_Header->charsize;
	newRenderer.CursorPosition.X = 0;
	newRenderer.Print( to_hstring( (uint64_t)0xF0 ) );
	newRenderer.CursorPosition.Y += newRenderer.PSF1_Font->psf1_Header->charsize;
	newRenderer.CursorPosition.X = 0;
	newRenderer.Print( to_hstring( (uint32_t)0xF0 ) );
	newRenderer.CursorPosition.Y += newRenderer.PSF1_Font->psf1_Header->charsize;
	newRenderer.CursorPosition.X = 0;
	newRenderer.Print( to_hstring( (uint16_t)0xF0 ) );
	newRenderer.CursorPosition.Y += newRenderer.PSF1_Font->psf1_Header->charsize;
	newRenderer.CursorPosition.X = 0;
	newRenderer.Print( to_hstring( (uint8_t)0xF0 ) );

	return;
}