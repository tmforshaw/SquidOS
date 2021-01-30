#include "Mouse.hpp"

#include "../Rendering/BasicRenderer.hpp"
#include "IO.hpp"

uint8_t MouseCycle = 0;
uint8_t MousePacket[4];
bool MousePacketReady = false;

Point MousePosition;

void MouseWait()
{
	uint64_t timeout = 100000;

	while ( timeout-- ) // Wait 100000 loops
	{
		if ( ( inb( 0x64 ) & 0b10 ) == 0 ) // Wait until the second bit is 0
			return;
	}
}

void MouseWaitInput()
{
	uint64_t timeout = 100000;

	while ( timeout-- ) // Wait 100000 loops
	{
		if ( inb( 0x64 ) & 0b1 ) // Wait until the first bit is 1
			return;
	}
}

void MouseWrite( uint8_t value )
{
	MouseWait();
	outb( 0x64, 0xD4 ); // Tell mouse that we are sending a command
	MouseWait();
	outb( 0x60, value );
}

uint8_t MouseRead()
{
	MouseWaitInput(); // Wait for input
	return inb( 0x60 );
}

void HandlePS2Mouse( uint8_t data )
{
	switch ( MouseCycle )
	{
	case 0:
		if ( MousePacketReady ) break;
		if ( ( data & 0b00001000 ) == 0 ) break; // Always 1 bit isn't 1 (Mouse is out of sync)

		MousePacket[0] = data;
		MouseCycle++;
		break;

	case 1:
		if ( MousePacketReady ) break;
		MousePacket[1] = data;
		MouseCycle++;
		break;

	case 2:
		if ( MousePacketReady ) break;
		MousePacket[2] = data;
		MousePacketReady = true; // Handled all packets
		MouseCycle = 0;
		break;

	default:
		break;
	}
}

void ProcessMousePacket()
{
	if ( !MousePacketReady ) return;
	MousePacketReady = false; // Allow it to be written to

	bool xNegative = MousePacket[0] & PS2XSign;
	bool yNegative = MousePacket[0] & PS2YSign;
	bool xOverflow = MousePacket[0] & PS2XOverflow;
	bool yOverflow = MousePacket[0] & PS2YOverflow;

	// X Position
	if ( xNegative )
		MousePacket[1] = 256 - MousePacket[1]; // Overflow the byte

	MousePosition.X += ( xNegative ? -1 : 1 ) * MousePacket[1];
	if ( xOverflow )
		MousePosition.X += ( xNegative ? -1 : 1 ) * 255;

	// Y Position
	if ( yNegative )
		MousePacket[2] = 256 - MousePacket[2]; // Overflow the byte

	MousePosition.Y -= ( yNegative ? -1 : 1 ) * MousePacket[2];
	if ( yOverflow )
		MousePosition.Y -= ( yNegative ? -1 : 1 ) * 255;

	// Check bounds

	// X
	if ( MousePosition.X < 0 ) MousePosition.X = 0;

	if ( MousePosition.X > GlobalRenderer->TargetFramebuffer->Width - PSF1_FontWidth )
		MousePosition.X = GlobalRenderer->TargetFramebuffer->Width - PSF1_FontWidth;

	// Y
	if ( MousePosition.Y < 0 ) MousePosition.Y = 0;

	if ( MousePosition.Y > GlobalRenderer->TargetFramebuffer->Height - GlobalRenderer->PSF1_Font->psf1_Header->charsize )
		MousePosition.Y = GlobalRenderer->TargetFramebuffer->Height - GlobalRenderer->PSF1_Font->psf1_Header->charsize;

	// Display

	GlobalRenderer->PutChar( 'A', MousePosition.X, MousePosition.Y );
}

void InitPS2Mouse()
{
	outb( 0x64, 0xA8 ); // Enable auxilliary device (mouse)
	MouseWait();

	outb( 064, 0x20 ); // Tells KB controller that we want to send a command to the mouse
	MouseWaitInput();  // Wait for input

	uint8_t status = inb( 0x60 ); // Get the status bytes
	status |= 0b10;				  // Set second bit

	MouseWait();
	outb( 0x64, 0x60 ); // Tell controller that we are going to send command to mouse
	MouseWait();
	outb( 0x60, status ); // Set correct bit in the "compaq" status byte

	MouseWrite( 0xF6 ); // Use default settings
	MouseRead();

	MouseWrite( 0xF4 ); // Enable mouse
	MouseRead();
}
