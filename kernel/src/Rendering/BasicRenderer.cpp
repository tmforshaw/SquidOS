#include "BasicRenderer.hpp"

#include "../Types/C_String.hpp"

BasicRenderer* GlobalRenderer;

BasicRenderer::BasicRenderer( Framebuffer* p_TargetFramebuffer, PSF1_FONT* p_PSF1_Font, uint32_t p_Colour )
{
	CursorPosition = { 0, 0 };

	TargetFramebuffer = p_TargetFramebuffer;
	PSF1_Font = p_PSF1_Font;
	Colour = p_Colour;
}

void BasicRenderer::PutChar( char chr, uint32_t xOff, uint32_t yOff )
{
	uint32_t* pixPtr = (uint32_t*)TargetFramebuffer->BaseAddress;
	char* fontPtr = (char*)PSF1_Font->glyphBuffer + ( chr * PSF1_Font->psf1_Header->charsize ); // Get address of the glyph buffer, then add on chr * charsize (Indexing glyph buffer)

	// Select the bit in the bitmap
	for ( unsigned long y = yOff; y < yOff + PSF1_Font->psf1_Header->charsize; y++ )
	{
		for ( unsigned long x = xOff; x < xOff + 8; x++ )
		{
			if ( ( *fontPtr & ( 0b10000000 >> ( x - xOff ) ) ) > 0 ) // Select the bit that we need
			{
				*(uint32_t*)( pixPtr + x + ( y * TargetFramebuffer->PixelsPerScanLine ) ) = Colour; // Set colour
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

		if ( CursorPosition.X + CursorPosition.Y * TargetFramebuffer->PixelsPerScanLine >= TargetFramebuffer->BufferSize )
			break;

		chr++;
	}
}

void BasicRenderer::Clear( uint32_t colour )
{
	uint64_t fbBase = (uint64_t)TargetFramebuffer->BaseAddress;
	uint64_t bytePerScanLine = TargetFramebuffer->PixelsPerScanLine * 4;
	uint64_t fbHeight = TargetFramebuffer->Height;

	for ( int verticalScanLine = 0; verticalScanLine < fbHeight; verticalScanLine++ )
	{
		uint64_t pixPtrBase = fbBase + ( bytePerScanLine * verticalScanLine ); // First pixel on row

		for ( uint32_t* pixPtr = (uint32_t*)pixPtrBase; pixPtr < (uint32_t*)( pixPtrBase + bytePerScanLine ); pixPtr++ )
			*pixPtr = colour;
	}

	GlobalRenderer->CursorPosition = { 0, 0 }; // Reset CursorPosition
}

void BasicRenderer::Endl()
{
	CursorPosition = { 0, CursorPosition.Y + PSF1_Font->psf1_Header->charsize };
}

// Drawing

void BasicRenderer::Line( Point p1, Point p2 )
{
	float dx = p2.X - p1.X;
	float dy = p2.Y - p1.Y;

	float angle = atan2( dy, dx );

	float stepX = cos( angle );
	float stepY = sin( angle );

	float distanceSqr = pow( dx, 2 ) + pow( dy, 2 );

	float x = (float)p1.X;
	float y = (float)p1.Y;

	for ( uint16_t i = 0; i * i < (uint16_t)distanceSqr; i++ )
	{
		x += stepX;
		y += stepY;

		if ( x + y * GlobalRenderer->TargetFramebuffer->PixelsPerScanLine * 4 < GlobalRenderer->TargetFramebuffer->BufferSize )
			*( (uint32_t*)( (uint32_t*)GlobalRenderer->TargetFramebuffer->BaseAddress + (uint32_t)x + (uint32_t)y * GlobalRenderer->TargetFramebuffer->PixelsPerScanLine * 4 ) ) = Colour;
		else
			break;
	}
}