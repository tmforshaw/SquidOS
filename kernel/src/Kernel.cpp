#include "KernelUtil.hpp"

extern "C" void _start( BootInfo* bootInfo )
{
	KernelInfo kernelInfo = InitialiseKernel( bootInfo );
	PageTableManager* PageTableManager = kernelInfo.pageTableManager;

	TextUI text = TextUI( { 300, 25 }, 320, 60, GREY );
	SelectedTextUI = &text;
	SelectedTextUI->Display();

	GlobalRenderer->Rect( { 250, 250 }, 150, 150 );
	GlobalRenderer->Rect( { 250, 425 }, 150, 150, false );
	GlobalRenderer->Circle( { 500, 325 }, 75, false );
	GlobalRenderer->Circle( { 500, 500 }, 75 );

	// GlobalSound->PlayFrequency( 1 );

	// clang-format off
	while ( true );
}