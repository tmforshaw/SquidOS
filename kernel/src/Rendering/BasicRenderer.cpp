#include "BasicRenderer.hpp"

#include "../Interfaces/TextInterface.hpp"
#include "../Types/C_String.hpp"

BasicRenderer* GlobalRenderer;

BasicRenderer::BasicRenderer( Framebuffer* p_TargetFramebuffer, PSF1_FONT* p_PSF1_Font, uint32_t p_Colour, uint32_t p_ClearColour )
{
	CursorPosition = { 0, 0 };

	TargetFramebuffer = p_TargetFramebuffer;
	PSF1_Font = p_PSF1_Font;
	Colour = p_Colour;
	ClearColour = p_ClearColour;
	ColourStack = p_Colour;

	MouseDrawn = false;
}

void BasicRenderer::PutChar( char chr, uint32_t xOff, uint32_t yOff )
{
	uint32_t* pixPtr = (uint32_t*)TargetFramebuffer->BaseAddress;
	char* fontPtr = (char*)PSF1_Font->glyphBuffer + ( chr * PSF1_Font->psf1_Header->charsize ); // Get address of the glyph buffer, then add on chr * charsize (Indexing glyph buffer)

	// Select the bit in the bitmap
	for ( unsigned long y = yOff; y < yOff + PSF1_Font->psf1_Header->charsize; y++ )
	{
		for ( unsigned long x = xOff; x < xOff + PSF1_FontWidth; x++ )
		{
			if ( ( *fontPtr & ( 0b10000000 >> ( x - xOff ) ) ) > 0 ) // Select the bit that we need
			{
				*(uint32_t*)( pixPtr + x + y * TargetFramebuffer->PixelsPerScanLine ) = Colour; // Set colour
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
		CursorPosition.X += PSF1_FontWidth;

		if ( CursorPosition.X + PSF1_FontWidth > TargetFramebuffer->Width )
		{
			CursorPosition.X = 0;
			CursorPosition.Y += PSF1_Font->psf1_Header->charsize;
		}

		chr++;
	}
}

void BasicRenderer::Print( String str )
{
	Print( str.GetCstr() );
}

void BasicRenderer::Print( char chr )
{
	PutChar( chr, CursorPosition.X, CursorPosition.Y );
	CursorPosition.X += PSF1_FontWidth;

	if ( CursorPosition.X + PSF1_FontWidth >= TargetFramebuffer->Width )
	{
		CursorPosition.X = 0;
		CursorPosition.Y += PSF1_Font->psf1_Header->charsize;
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

void BasicRenderer::ClearChar( uint16_t xOff, uint16_t yOff, uint32_t colour )
{
	for ( unsigned long y = yOff; y < yOff + PSF1_Font->psf1_Header->charsize; y++ )
		for ( unsigned long x = xOff; x < xOff + PSF1_FontWidth; x++ )
			*(uint32_t*)( (uint32_t*)TargetFramebuffer->BaseAddress + x + y * TargetFramebuffer->PixelsPerScanLine ) = colour;
}

void BasicRenderer::Endl( uint16_t amt )
{
	for ( uint16_t i = 0; i < amt; i++ )
		CursorPosition = { 0, CursorPosition.Y + PSF1_Font->psf1_Header->charsize };
}

// Mouse

void BasicRenderer::ClearMouseCursor( Point pos, uint8_t* MouseCursor )
{
	if ( !MouseDrawn ) return;

	int xMax = 16;
	int yMax = 16;
	int diffX = TargetFramebuffer->Width - pos.X;
	int diffY = TargetFramebuffer->Height - pos.Y;

	// Prevent going over frame buffer
	if ( diffX < xMax ) xMax = diffX;
	if ( diffY < yMax ) yMax = diffY;

	for ( int y = 0; y < yMax; y++ )
	{
		for ( int x = 0; x < xMax; x++ )
		{
			int bit = y * 16 + x; // Index for the MousePointer array
			int byte = bit / 8;

			if ( ( MouseCursor[byte] ) & ( 0b10000000 >> ( x % 8 ) ) )
			{
				if ( GetPix( { (uint32_t)pos.X + (uint32_t)x, (uint32_t)pos.Y + (uint32_t)y } ) == MouseCursorBufferNext[x + y * 16] ) // If it hasn't been written over
				{
					PushColour( MouseCursorBuffer[x + y * 16] );
					PutPix( { (uint32_t)pos.X + (uint32_t)x, (uint32_t)pos.Y + (uint32_t)y } );
					PopColour();
				}
			}
		}
	}
}

void BasicRenderer::DrawOverlayMouseCursor( Point pos, uint8_t* MouseCursor, uint32_t colour )
{
	int xMax = 16;
	int yMax = 16;
	int diffX = TargetFramebuffer->Width - pos.X;
	int diffY = TargetFramebuffer->Height - pos.Y;

	// Prevent going over frame buffer
	if ( diffX < xMax ) xMax = diffX;
	if ( diffY < yMax ) yMax = diffY;

	for ( int y = 0; y < yMax; y++ )
	{
		for ( int x = 0; x < xMax; x++ )
		{
			int bit = y * 16 + x; // Index for the MousePointer array
			int byte = bit / 8;

			if ( ( MouseCursor[byte] ) & ( 0b10000000 >> ( x % 8 ) ) )
			{
				MouseCursorBuffer[x + y * yMax] = GetPix( { (uint32_t)pos.X + (uint32_t)x, (uint32_t)pos.Y + (uint32_t)y } ); // Save previous pixel

				PushColour( colour );
				PutPix( { (uint32_t)pos.X + (uint32_t)x, (uint32_t)pos.Y + (uint32_t)y } );
				PopColour();

				MouseCursorBufferNext[x + y * yMax] = GetPix( { (uint32_t)pos.X + (uint32_t)x, (uint32_t)pos.Y + (uint32_t)y } ); // Save current pixel
			}
		}
	}

	MouseDrawn = true;
}

// Colour

void BasicRenderer::PushColour( uint32_t p_Colour )
{
	ColourStack = Colour;
	Colour = p_Colour;
}

void BasicRenderer::PopColour() { Colour = ColourStack; }

// Drawing

void BasicRenderer::PutPix( PointU p )
{
	uint32_t pixPtr = p.X + p.Y * TargetFramebuffer->PixelsPerScanLine;

	if ( pixPtr < TargetFramebuffer->BufferSize && pixPtr >= 0 )
		*( (uint32_t*)TargetFramebuffer->BaseAddress + pixPtr ) = Colour;
}

uint32_t BasicRenderer::GetPix( PointU p )
{
	uint32_t pixPtr = p.X + p.Y * TargetFramebuffer->PixelsPerScanLine;

	if ( pixPtr < TargetFramebuffer->BufferSize && pixPtr >= 0 )
		return *( (uint32_t*)TargetFramebuffer->BaseAddress + pixPtr );

	return DEFAULT_BG_COLOUR;
}

void BasicRenderer::Line( PointU p1, PointU p2 )
{
	float dx = (float)p2.X - (float)p1.X;
	float dy = (float)p2.Y - (float)p1.Y;

	float angle = atan2( dy, dx );

	float stepX = cos( angle );
	float stepY = sin( angle );

	float distance = sqrt( pow( dx, 2 ) + pow( dy, 2 ) );

	float x = (float)p1.X;
	float y = (float)p1.Y;

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

void BasicRenderer::Rect( PointU pos, uint16_t width, uint16_t height, bool fill )
{
	if ( fill )
	{
		// Use scan lines to fill the square
		for ( uint16_t y = pos.Y; y < pos.Y + height; y++ ) // #TODO# Optimise this code
			for ( uint16_t x = pos.X; x < pos.X + width; x++ )
				PutPix( { x, y } );
	}
	else
	{
		// Just draw the edges
		Line( { pos.X, pos.Y }, { pos.X + width, pos.Y } );
		Line( { pos.X + width, pos.Y }, { pos.X + width, pos.Y + height } );
		Line( { pos.X + width, pos.Y + height }, { pos.X, pos.Y + height } );
		Line( { pos.X, pos.Y + height }, { pos.X, pos.Y } );
	}
}

void BasicRenderer::Circle( PointU pos, uint16_t r, bool fill )
{
	if ( fill )
	{
		// Filling the circle (Inefficient algorithm)

		int16_t r2 = r * r;
		int16_t area = r2 << 2;
		int16_t rr = r << 1;

		for ( int16_t i = 0; i < area; i++ )
		{
			int16_t tx = ( i % rr ) - r;
			int16_t ty = ( i / rr ) - r;

			if ( tx * tx + ty * ty <= r2 )
				PutPix( { ( uint32_t )( pos.X + tx ), ( uint32_t )( pos.Y + ty ) } );
		}
	}
	else
	{
		// Draw the outer edges of the circle

		// Bresenham circle drawing algorithm
		int16_t x = 0, y = r;
		int16_t decisionParam = 3 - 2 * r;

		DisplayBresenham( pos, x, y );

		while ( y > x )
		{
			x++;

			if ( decisionParam > 0 )
			{
				y--;
				decisionParam += 4 * ( x - y ) + 10;
			}
			else
				decisionParam += 4 * x + 6;

			DisplayBresenham( pos, x, y );
		}
	}
}

void BasicRenderer::DisplayBresenham( PointU pos, int16_t x, int16_t y )
{
	// Mirror the pixel to all 8 octants

	PutPix( { ( uint32_t )( pos.X + x ), ( uint32_t )( pos.Y + y ) } );
	PutPix( { ( uint32_t )( pos.X - x ), ( uint32_t )( pos.Y + y ) } );
	PutPix( { ( uint32_t )( pos.X + x ), ( uint32_t )( pos.Y - y ) } );
	PutPix( { ( uint32_t )( pos.X - x ), ( uint32_t )( pos.Y - y ) } );
	PutPix( { ( uint32_t )( pos.X + y ), ( uint32_t )( pos.Y + x ) } );
	PutPix( { ( uint32_t )( pos.X - y ), ( uint32_t )( pos.Y + x ) } );
	PutPix( { ( uint32_t )( pos.X + y ), ( uint32_t )( pos.Y - x ) } );
	PutPix( { ( uint32_t )( pos.X - y ), ( uint32_t )( pos.Y - x ) } );
}

void BasicRenderer::Sin( PointU pos, uint16_t width, uint16_t step, uint16_t amplitude, float timePeriod, float xOffset )
{
	uint16_t minimumX = pos.X;
	uint16_t aX = 0;
	float omega = 1 / timePeriod; // Should be TAU / T but that makes the numbers large

	if ( minimumX + pos.Y * TargetFramebuffer->PixelsPerScanLine < 0 ) minimumX = 0;

	while ( aX + minimumX < min( ( uint32_t )( minimumX + width ), TargetFramebuffer->Width ) )
	{
		PutPix( { ( uint32_t )( aX + minimumX ), pos.Y + ( uint32_t )( amplitude * -sin( omega * (float)( aX + xOffset ) ) ) } ); // The xOffset only affects the y-value

		// x += step;
		aX += step;
	}
}

void BasicRenderer::Cos( PointU pos, uint16_t width, uint16_t step, uint16_t amplitude, float timePeriod, float xOffset )
{
	Sin( pos, width, step, amplitude, timePeriod, ( Math::PI_2 + xOffset ) * timePeriod ); // Offset sin by PI/2
}