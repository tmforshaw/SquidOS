#include "KernelUtil.hpp"

extern "C" void _start( BootInfo* bootInfo )
{
	KernelInfo kernelInfo = InitialiseKernel( bootInfo );
	PageTableManager* PageTableManager = kernelInfo.pageTableManager;

	GlobalRenderer->Print( "Kernel Initialised Successfully" );

	int* test = (int*)( 0x90000000000 );
	*test = 2;

	// clang-format off
	while ( true );
}