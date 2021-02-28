#include "Mouse.hpp"

#include "../Rendering/BasicRenderer.hpp"
#include "IO.hpp"

uint8_t MouseCycle = 0;
uint8_t MousePacket[4];
bool	MousePacketReady = false;

Point MousePosition;
Point MousePositionPrev;

// This is the shape of the mouse
uint8_t MousePointer[] = { // 16-bit by 16-bit array
	0b11111111, 0b11100000,
	0b11111111, 0b10000000,
	0b11111110, 0b00000000,
	0b11111100, 0b00000000,
	0b11111000, 0b00000000,
	0b11110000, 0b00000000,
	0b11100000, 0b00000000,
	0b11000000, 0b00000000,
	0b11000000, 0b00000000,
	0b10000000, 0b00000000,
	0b10000000, 0b00000000,
	0b00000000, 0b00000000,
	0b00000000, 0b00000000,
	0b00000000, 0b00000000,
	0b00000000, 0b00000000,
	0b00000000, 0b00000000
};

bool LeftButtonDown	  = false;
bool MiddleButtonDown = false;
bool RightButtonDown  = false;

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
	ProcessMousePacket();
	static bool skip = true;

	if ( skip )
	{
		skip = false;
		return;
	}

	switch ( MouseCycle )
	{
	case 0:
		if ( ( data & 0b00001000 ) == 0 ) break; // Always 1 bit isn't 1 (Mouse is out of sync)
		MousePacket[0] = data;
		MouseCycle++;
		break;

	case 1:
		MousePacket[1] = data;
		MouseCycle++;
		break;

	case 2:
		MousePacket[2]	 = data;
		MousePacketReady = true; // Handled all packets
		MouseCycle		 = 0;
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
	if ( MousePosition.X > GlobalRenderer->TargetFramebuffer->Width - 1 ) MousePosition.X = GlobalRenderer->TargetFramebuffer->Width - 1;

	// Y
	if ( MousePosition.Y < 0 ) MousePosition.Y = 0;
	if ( MousePosition.Y > GlobalRenderer->TargetFramebuffer->Height - 1 ) MousePosition.Y = GlobalRenderer->TargetFramebuffer->Height - 1;

	// Display

	GlobalRenderer->ClearMouseCursor( MousePositionPrev, MousePointer );
	GlobalRenderer->DrawOverlayMouseCursor( MousePosition, MousePointer );

	if ( MousePacket[0] & PS2LeftButton && !LeftButtonDown ) // Left button pressed
		LeftButtonDown = true;
	else if ( LeftButtonDown )
		LeftButtonDown = false;

	if ( MousePacket[0] & PS2MiddleButton && !MiddleButtonDown ) // Middle button pressed
		MiddleButtonDown = true;
	else if ( LeftButtonDown )
		MiddleButtonDown = false;

	if ( MousePacket[0] & PS2RightButton && !RightButtonDown ) // Right button pressed
		RightButtonDown = true;
	else if ( RightButtonDown )
		RightButtonDown = false;

	// if ( RightButtonDown )
	// 	GlobalRenderer->PushColour( BLUE );

	// if ( MiddleButtonDown )
	// 	GlobalRenderer->PushColour( RED );

	// if ( LeftButtonDown )
	// 	GlobalRenderer->Rect( { (uint32_t)MousePosition.X, (uint32_t)MousePosition.Y }, 4, 4, true );

	// GlobalRenderer->PopColour();

	MousePacketReady  = false;
	MousePositionPrev = MousePosition;
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
