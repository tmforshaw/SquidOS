#include "BasicRenderer.hpp"

BasicRenderer::BasicRenderer( Framebuffer* p_TargetFramebuffer, PSF1_FONT* p_PSF1_Font, unsigned int p_Colour )
{
	CursorPosition = { 0, 0 };

	TargetFramebuffer = p_TargetFramebuffer;
	PSF1_Font = p_PSF1_Font;
	Colour = p_Colour;
}

void BasicRenderer::PutChar( char chr, unsigned int xOff, unsigned int yOff )
{
	unsigned int* pixPtr = (unsigned int*)TargetFramebuffer->BaseAddress;
	char* fontPtr = (char*)PSF1_Font->glyphBuffer + ( chr * PSF1_Font->psf1_Header->charsize ); // Get address of the glyph buffer, then add on chr * charsize (Indexing glyph buffer)

	// Select the bit in the bitmap
	for ( unsigned long y = yOff; y < yOff + PSF1_Font->psf1_Header->charsize; y++ )
	{
		for ( unsigned long x = xOff; x < xOff + 8; x++ )
		{
			if ( ( *fontPtr & ( 0b10000000 >> ( x - xOff ) ) ) > 0 ) // Select the bit that we need
			{
				*(unsigned int*)( pixPtr + x + ( y * TargetFramebuffer->PixelsPerScanLine ) ) = Colour; // Set colour
			}
		}
		fontPtr++;
	}
}

void BasicRenderer::Print( const char* str )
{
	char* chr = (char*)str;

	while ( *chr != '\0' )
	{
		PutChar( *chr, CursorPosition.X, CursorPosition.Y );
		CursorPosition.X += 8;
		if ( CursorPosition.X + 8 > TargetFramebuffer->Width )
		{
			CursorPosition.X = 0;
			CursorPosition.Y += PSF1_Font->psf1_Header->charsize;
		}
		chr++;
	}
}