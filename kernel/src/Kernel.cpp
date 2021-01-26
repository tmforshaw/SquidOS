#include "KernelUtil.hpp"

extern "C" void _start( BootInfo* bootInfo )
{
	KernelInfo kernelInfo = InitialiseKernel( bootInfo );
	PageTableManager* PageTableManager = kernelInfo.pageTableManager;

	GlobalRenderer->Print( "Kernel Initialised Successfully" );
	GlobalRenderer->Endl( 2 );

	PrintMemoryDebug();
	GlobalRenderer->Endl();

	GlobalRenderer->Rect( { 250, 250 }, 150, 150 );

	// clang-format off
	while ( true );
}