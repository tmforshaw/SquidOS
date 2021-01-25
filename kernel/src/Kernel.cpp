#include "KernelUtil.hpp"

extern "C" void _start( BootInfo* bootInfo )
{
	KernelInfo kernelInfo = InitialiseKernel( bootInfo );
	PageTableManager* PageTableManager = kernelInfo.pageTableManager;

	// GlobalRenderer->Colour = GREEN;

	// const char* message = "This is a test";

	// GlobalRenderer->Colour = RGBA_ToHex( 255, 0, 255, 255 );
	// GlobalRenderer->Print( message );
	// GlobalRenderer->Endl();
	// GlobalRenderer->Colour = RGBA_ToHex( 0, 255, 0, 255 );
	// GlobalRenderer->Print( message );
	// GlobalRenderer->Endl();
	// GlobalRenderer->Colour = RGBA_ToHex( 0, 255, 255, 255 );
	// GlobalRenderer->Print( message );
	// GlobalRenderer->Endl();
	// GlobalRenderer->Colour = RGBA_ToHex( 0, 0, 255, 255 );
	// GlobalRenderer->Print( message );
	// GlobalRenderer->Endl();

	GlobalRenderer->Print( "Kernel Initialised Successfully" );
	GlobalRenderer->Endl();

	GlobalRenderer->Line( { GlobalRenderer->TargetFramebuffer->Width / 2, GlobalRenderer->TargetFramebuffer->Height / 2 }, { ( uint32_t )( GlobalRenderer->TargetFramebuffer->Width * 0.9f ), GlobalRenderer->TargetFramebuffer->Height / 2 } );

	// GlobalRenderer->Line( { 25, 50 }, { GlobalRenderer->TargetFramebuffer->Width, 50 } );

	GlobalRenderer->Print( to_string( (uint64_t)GlobalRenderer->TargetFramebuffer->Width ) );

	// clang-format off
	while ( true );
}