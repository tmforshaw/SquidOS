#include "KernelUtil.hpp"

extern "C" void _start( BootInfo* bootInfo )
{
	KernelInfo kernelInfo = InitialiseKernel( bootInfo );
	PageTableManager* PageTableManager = kernelInfo.pageTableManager;

	// GlobalCommand.Display(); // Show the CLI

	// GlobalRenderer->Print( to_string( (uint64_t)GlobalCommand.GetTextUI().AbsoluteCursorPosition.X ) );

	TextUI txt = TextUI( { 300, 25 }, 200, 75 );
	txt.Display();
	SelectedTextUI = &txt;

	GlobalRenderer->Rect( { 250, 250 }, 150, 150 );
	GlobalRenderer->Rect( { 250, 425 }, 150, 150, false );
	GlobalRenderer->Circle( { 500, 325 }, 75, false );
	GlobalRenderer->Circle( { 500, 500 }, 75 );

	// GlobalSound->PlayFrequency( 1 );

	// clang-format off
	while ( true );
}