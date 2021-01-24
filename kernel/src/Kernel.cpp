#include "KernelUtil.hpp"

extern "C" void _start( BootInfo* bootInfo )
{
	KernelInfo kernelInfo = InitialiseKernel( bootInfo );
	PageTableManager* PageTableManager = kernelInfo.pageTableManager;

	BasicRenderer newRenderer = BasicRenderer( bootInfo->framebuffer, bootInfo->psf1_font );

	newRenderer.Print( "OIOIOI" );

	while ( true )
		;
}