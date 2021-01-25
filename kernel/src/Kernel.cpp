#include "KernelUtil.hpp"

extern "C" void _start( BootInfo* bootInfo )
{
	KernelInfo kernelInfo = InitialiseKernel( bootInfo );
	PageTableManager* PageTableManager = kernelInfo.pageTableManager;

	GlobalRenderer->Print( "Kernel Initialised Successfully" );
	GlobalRenderer->Endl();

	GlobalRenderer->Line( { 350, 251 }, { GlobalRenderer->TargetFramebuffer->Width, 250 } );

	// clang-format off
	while ( true );
}