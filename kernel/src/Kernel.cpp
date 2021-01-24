#include "KernelUtil.hpp"

extern "C" void _start( BootInfo* bootInfo )
{
	KernelInfo kernelInfo = InitialiseKernel( bootInfo );
	PageTableManager* PageTableManager = kernelInfo.pageTableManager;
	BasicRenderer renderer = BasicRenderer( bootInfo->framebuffer, bootInfo->psf1_font );

	renderer.Print( "Kernel Initialised Successfully" );

	// clang-format off
	while ( true );
}