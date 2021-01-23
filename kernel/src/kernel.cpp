#include "BasicRenderer.hpp"
#include "Bitmap.hpp"
#include "C_String.hpp"
#include "EFI_Memory.hpp"
#include "Memory.hpp"
#include "PageFrameAllocator.hpp"

struct BootInfo
{
	Framebuffer* framebuffer;
	PSF1_FONT* psf1_font;
	EFI_MEMORY_DESCRIPTOR* m_Map;
	uint64_t m_MapSize;
	uint64_t m_MapDescriptorSize;
};

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

extern "C" void _start( BootInfo* bootInfo )
{
	BasicRenderer newRenderer = BasicRenderer( bootInfo->framebuffer, bootInfo->psf1_font );
	newRenderer.CursorPosition = { 0, 0 };

	uint64_t m_MapEntries = bootInfo->m_MapSize / bootInfo->m_MapDescriptorSize;

	newRenderer.Print( "Total RAM: " );
	newRenderer.Colour = 0xFFFF00FF;
	newRenderer.Print( to_string( GetMemorySize( bootInfo->m_Map, m_MapEntries, bootInfo->m_MapDescriptorSize ) / 1024 ) );
	newRenderer.Colour = 0xFF00FFFF;
	newRenderer.Print( " KiB" );
	newRenderer.CursorPosition = { 0, newRenderer.CursorPosition.Y + 16 }; // New Line

	PageFrameAllocator newAllocator;
	newAllocator.ReadEFIMemoryMap( bootInfo->m_Map, bootInfo->m_MapSize, bootInfo->m_MapDescriptorSize );

	// Lock the pages where the kernel is located
	uint64_t kernelSize = ( uint64_t )( &_KernelEnd ) - ( uint64_t )( &_KernelStart );
	uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;
	newAllocator.LockPages( &_KernelStart, kernelPages );

	newRenderer.Print( "Free RAM: " );
	newRenderer.Colour = 0xFFFF00FF;
	newRenderer.Print( to_string( newAllocator.GetFreeRAM() / 1024 ) );
	newRenderer.Colour = 0xFF00FFFF;
	newRenderer.Print( " KiB" );
	newRenderer.CursorPosition = { 0, newRenderer.CursorPosition.Y + 16 }; // New Line
	newRenderer.Print( "Used RAM: " );
	newRenderer.Colour = 0xFFFF00FF;
	newRenderer.Print( to_string( newAllocator.GetUsedRAM() / 1024 ) );
	newRenderer.Colour = 0xFF00FFFF;
	newRenderer.Print( " KiB" );
	newRenderer.CursorPosition = { 0, newRenderer.CursorPosition.Y + 16 }; // New Line
	newRenderer.Print( "Reserved RAM: " );
	newRenderer.Colour = 0xFFFF00FF;
	newRenderer.Print( to_string( newAllocator.GetReservedRAM() / 1024 ) );
	newRenderer.Colour = 0xFF00FFFF;
	newRenderer.Print( " KiB" );
	newRenderer.CursorPosition = { 0, newRenderer.CursorPosition.Y + 16 }; // New Line

	for ( int t = 0; t < 5; t++ )
	{
		void* address = newAllocator.RequestPage();

		newRenderer.Print( to_hstring( (uint64_t)address ) );
		newRenderer.CursorPosition = { 0, newRenderer.CursorPosition.Y + 16 }; // New Line
	}

	for ( int i = 0; i < m_MapEntries; i++ )
	{
		EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)( (uint64_t)bootInfo->m_Map + ( i * bootInfo->m_MapDescriptorSize ) );
		newRenderer.CursorPosition = { 0, newRenderer.CursorPosition.Y + 16 }; // New Line
		newRenderer.Print( EFI_MEMORY_TYPE_STRINGS[desc->type] );
		newRenderer.Colour = 0xFFFF00FF;
		newRenderer.Print( " " );
		newRenderer.Print( to_string( desc->numPages * 4096 / 1024 ) ); // Print size in kb
		newRenderer.Print( " KiB" );
		newRenderer.Colour = 0xFF00FFFF;
	}

	return;
}