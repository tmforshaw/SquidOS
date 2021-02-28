#include "Panic.hpp"

#include "../Rendering/BasicRenderer.hpp"

void Panic( const char* panicMessage )
{
	GlobalRenderer->Clear( 0x00FF0000 ); // Clear to red

	GlobalRenderer->Colour = 0x00000000;
	GlobalRenderer->Print( "Kernel Panic" );
	GlobalRenderer->Endl();

	GlobalRenderer->Print( panicMessage );
	GlobalRenderer->Endl();
}