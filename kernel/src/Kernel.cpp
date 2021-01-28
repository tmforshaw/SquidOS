#include "KernelUtil.hpp"

extern "C" void _start( BootInfo* bootInfo )
{
	KernelInfo kernelInfo = InitialiseKernel( bootInfo );
	PageTableManager* PageTableManager = kernelInfo.pageTableManager;

	GlobalCommand.Display(); // Show the CLI

	GlobalRenderer->Rect( { 250, 250 }, 150, 150 );
	GlobalRenderer->Rect( { 250, 425 }, 150, 150, false );
	GlobalRenderer->Circle( { 500, 325 }, 75, false );
	GlobalRenderer->Circle( { 500, 500 }, 75 );

	// GlobalSound->PlayFrequency( 1 );

	// clang-format off
	while ( true );
}


// The text wont empty when I tell it to