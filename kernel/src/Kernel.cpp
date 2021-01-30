#include "KernelUtil.hpp"

extern "C" void _start( BootInfo* bootInfo )
{
	KernelInfo kernelInfo = InitialiseKernel( bootInfo );
	PageTableManager* PageTableManager = kernelInfo.pageTableManager;

	GlobalCommand.Display(); // Show the CLI

	// String test = "Command Param1 Param2";
	// GlobalRenderer->Print( test );
	// GlobalRenderer->Endl();
	// GlobalRenderer->Print( test.GetWord( 0 ) );
	// GlobalRenderer->Print( "_" );
	// GlobalRenderer->Endl();

	GlobalRenderer->Rect( { 250, 250 }, 150, 150 );
	GlobalRenderer->Rect( { 250, 425 }, 150, 150, false );
	GlobalRenderer->Circle( { 500, 325 }, 75, false );
	GlobalRenderer->Circle( { 500, 500 }, 75 );

	GlobalRenderer->Cos( { 650, 225 }, GlobalRenderer->TargetFramebuffer->Width / 2, 1, 150, 60.0f );

	// GlobalSound->PlayFrequency( 1 );

	// Kernel Loop
	while ( true )
	{
		ProcessMousePacket();
	}

	while ( true ) {}
}

// TEXT INTERFACE ETC