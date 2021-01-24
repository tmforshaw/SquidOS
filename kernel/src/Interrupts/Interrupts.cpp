#include "Interrupts.hpp"

#include "../Rendering/BasicRenderer.hpp"

__attribute__( ( interrupt ) ) void PageFault_Handler( struct interrupt_frame* frame )
{
	GlobalRenderer->Print( "Page fault detected." );

	// clang-format off
    while(true); // Halt execution
}
