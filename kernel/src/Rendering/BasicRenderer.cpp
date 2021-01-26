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
		// Out of bounds
		if ( CursorPosition.X + CursorPosition.Y * TargetFramebuffer->PixelsPerScanLine >= TargetFramebuffer->BufferSize || CursorPosition.X + CursorPosition.Y * TargetFramebuffer->PixelsPerScanLine < 0 )
			break;

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

void BasicRenderer::Endl( uint16_t amt )
{
	for ( uint16_t i = 0; i < amt; i++ )
		CursorPosition = { 0, CursorPosition.Y + PSF1_Font->psf1_Header->charsize };
}

// Drawing

void BasicRenderer::Line( Point p1, Point p2 )
{
	float dx = (float)p2.X - (float)p1.X;
	float dy = (float)p2.Y - (float)p1.Y;

	float angle = atan2( dy, dx );

	float stepX = cos( angle );
	float stepY = sin( angle );

	float distance = sqrt( pow( dx, 2 ) + pow( dy, 2 ) );

	float x = (float)p1.X;
	float y = (float)p1.Y;

	// GlobalRenderer->Print( "Delta x: " );
	// GlobalRenderer->Print( to_string( dx, 5 ) );
	// GlobalRenderer->Endl();
	// GlobalRenderer->Print( "Delta y: " );
	// GlobalRenderer->Print( to_string( dy, 5 ) );
	// GlobalRenderer->Endl();
	// GlobalRenderer->Print( "Angle (Deg): " );
	// GlobalRenderer->Print( to_string( radiansToDegrees( angle ), 5 ) );
	// GlobalRenderer->Endl();
	GlobalRenderer->Print( "Distance: " );
	GlobalRenderer->Print( to_string( distance, 5 ) );
	GlobalRenderer->Endl();
	GlobalRenderer->Print( "Cos(angle): " );
	GlobalRenderer->Print( to_string( stepX, 5 ) );
	GlobalRenderer->Endl();
	// GlobalRenderer->Print( "Sin(angle): " );
	// GlobalRenderer->Print( to_string( stepY, 5 ) );
	// GlobalRenderer->Endl();

	uint32_t pixPtr;

	for ( uint16_t i = 0; i < (int16_t)distance; i++ )
	{
		x += cos( angle );
		y += sin( angle );

		pixPtr = (uint32_t)x + (uint32_t)y * TargetFramebuffer->PixelsPerScanLine;

		// See if it fits within the buffer then draw it
		if ( pixPtr < TargetFramebuffer->BufferSize && pixPtr >= 0 )
			*( (uint32_t*)( (uint32_t*)TargetFramebuffer->BaseAddress + pixPtr ) ) = Colour;
		else
			break;
	}
}

void BasicRenderer::Rect( Point pos, uint16_t width, uint16_t height )
{
	Line( { pos.X, pos.Y }, { pos.X + width, pos.Y } );
	Line( { pos.X + width, pos.Y }, { pos.X + width, pos.Y + height } );
	Line( { pos.X + width, pos.Y + height }, { pos.X, pos.Y + height } );
	Line( { pos.X, pos.Y + height }, { pos.X, pos.Y } );
}