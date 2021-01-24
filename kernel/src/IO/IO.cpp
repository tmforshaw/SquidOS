#include "IO.hpp"

void outb( uint16_t port, uint8_t byte )
{
	asm volatile( "outb %0, %1"
				  :
				  : "a"( byte ), "Nd"( port ) );
}

uint8_t inb( uint16_t port )
{
	uint8_t returnByte;
	asm volatile( "inb %1, %0"
				  : "=a"( returnByte )
				  : "Nd"( port ) );
	return returnByte;
}

void io_wait()
{
	asm volatile( "outb %%al, $0x80"
				  :
				  : "a"( 0 ) ); // Sent byte to unused port
}