#include "BasicRenderer.hpp"
#include "C_String.hpp"
#include "EFI_Memory.hpp"

struct BootInfo
{
	Framebuffer* framebuffer;
	PSF1_FONT* psf1_font;
	void* m_Map;
	uint64_t m_MapSize;
	uint64_t m_MapDescriptorSize;
};

extern "C" void _start( BootInfo* bootInfo )
{
	BasicRenderer newRenderer = BasicRenderer( bootInfo->framebuffer, bootInfo->psf1_font );
	newRenderer.CursorPosition = { 0, 0 };

	newRenderer.CursorPosition = { 0, newRenderer.CursorPosition.Y + 16 }; // New Line

	uint64_t m_MapEntries = bootInfo->m_MapSize / bootInfo->m_MapDescriptorSize;

	for ( int i = 0; i < m_MapEntries; i++ )
	{
		EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)( (uint64_t)bootInfo->m_Map + ( i * bootInfo->m_MapDescriptorSize ) );
		newRenderer.CursorPosition = { 0, newRenderer.CursorPosition.Y + 16 }; // New Line
		newRenderer.Print( EFI_MEMORY_TYPE_STRINGS[desc->type] );
		newRenderer.Colour = 0xFFFF00FF;
		newRenderer.Print( " " );
		newRenderer.Print( to_string( desc->numPages * 4096 / 1024 ) ); // Print size in kb
		newRenderer.Print( " KB" );
		newRenderer.Colour = 0xFF00FFFF;
	}

	return;
}