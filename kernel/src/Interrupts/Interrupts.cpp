#include "Interrupts.hpp"

#include "../IO/IO.hpp"
#include "../Panic/Panic.hpp"
#include "../Rendering/BasicRenderer.hpp"

__attribute__( ( interrupt ) ) void PageFault_Handler( struct interrupt_frame* frame )
{
	Panic( "Page Fault Detected." );

	// clang-format off
    while(true); // Halt execution
	// clang-format on
}

__attribute__( ( interrupt ) ) void DoubleFault_Handler( struct interrupt_frame* frame )
{
	Panic( "Double Fault Detected." );

	// clang-format off
    while(true); // Halt execution
	// clang-format on
}

__attribute__( ( interrupt ) ) void GPFault_Handler( struct interrupt_frame* frame )
{
	Panic( "General Protection Fault Detected." );

	// clang-format off
    while(true); // Halt execution
	// clang-format on
}

__attribute__( ( interrupt ) ) void KeyboardInt_Handler( struct interrupt_frame* frame )
{
	GlobalRenderer->Print( "Pressed " );

	uint8_t scancode = inb( 0x60 ); // Port for PS2 keyboard
	PIC_EndMaster();
}

void RemapPIC()
{
	uint8_t a1, a2;

	a1 = inb( PIC1_DATA );
	io_wait();
	a2 = inb( PIC2_DATA );
	io_wait();

	outb( PIC1_COMMAND, ICW1_INIT | ICW1_ICW4 );
	io_wait();
	outb( PIC2_COMMAND, ICW1_INIT | ICW1_ICW4 );
	io_wait();

	outb( PIC1_DATA, 0x20 );
	io_wait();
	outb( PIC2_DATA, 0x28 );
	io_wait();

	outb( PIC1_DATA, 4 );
	io_wait();
	outb( PIC2_DATA, 2 );
	io_wait();

	outb( PIC1_DATA, ICW4_8086 );
	io_wait();
	outb( PIC2_DATA, ICW4_8086 );
	io_wait();

	outb( PIC1_DATA, a1 );
	io_wait();
	outb( PIC2_DATA, a2 );
}

void PIC_EndMaster()
{
	outb( PIC1_COMMAND, PIC_EOI );
}

void PIC_EndSlave()
{
	outb( PIC2_COMMAND, PIC_EOI );
	outb( PIC1_COMMAND, PIC_EOI );
}