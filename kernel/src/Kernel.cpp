#include "KernelUtil.hpp"

extern "C" void _start( BootInfo* bootInfo )
{
	KernelInfo kernelInfo = InitialiseKernel( bootInfo );
	PageTableManager* PageTableManager = kernelInfo.pageTableManager;

	GlobalRenderer->Print( "Kernel Initialised Successfully" );
	GlobalRenderer->Endl( 2 );

	PrintMemoryDebug();
	GlobalRenderer->Endl();

	// GlobalRenderer->Rect( { 150, 150 }, 150, 150 );

	GlobalRenderer->Line( { 350, 150 }, { 750, 150 } );
	GlobalRenderer->Line( { 750, 250 }, { 350, 250 } );

	// clang-format off
	while ( true );
}