#include "KernelUtil.hpp"

extern "C" void _start( BootInfo* bootInfo )
{
	KernelInfo kernelInfo = InitialiseKernel( bootInfo );
	PageTableManager* PageTableManager = kernelInfo.pageTableManager;

	GlobalRenderer->Print( "Kernel Initialised Successfully" );
	GlobalRenderer->Endl();

	// uint32_t y = 20;

	// for ( uint32_t x = 0; x < GlobalRenderer->TargetFramebuffer->Width / 2; x++ )
	// {
	// 	*(uint32_t*)( (uint32_t*)GlobalRenderer->TargetFramebuffer->BaseAddress + x + y * GlobalRenderer->TargetFramebuffer->PixelsPerScanLine ) = GlobalRenderer->Colour;
	// }

	// GlobalRenderer->Print( "Done" );

	// GlobalRenderer->Line( { 25, 25 }, { 250, 25 } );

	GlobalRenderer->Print( to_string( sqrt( 16.0f ) ) );

	// clang-format off
	while ( true );
}