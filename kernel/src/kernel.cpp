#include "Memory/EFI_Memory.hpp"
#include "Memory/Memory.hpp"
#include "Paging/PageFrameAllocator.hpp"
#include "Paging/PageMapIndexer.hpp"
#include "Paging/PageTableManager.hpp"
#include "Paging/Paging.hpp"
#include "Rendering/BasicRenderer.hpp"
#include "Types/Bitmap.hpp"
#include "Types/C_String.hpp"

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

	uint64_t m_MapEntries = bootInfo->m_MapSize / bootInfo->m_MapDescriptorSize;

	GlobalAllocator = PageFrameAllocator(); // Instantiate
	GlobalAllocator.ReadEFIMemoryMap( bootInfo->m_Map, bootInfo->m_MapSize, bootInfo->m_MapDescriptorSize );

	// Lock the pages where the kernel is located
	uint64_t kernelSize = ( uint64_t )( &_KernelEnd ) - ( uint64_t )( &_KernelStart );
	uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;
	GlobalAllocator.LockPages( &_KernelStart, kernelPages );

	PageTable* PML4 = (PageTable*)GlobalAllocator.RequestPage();
	memset( PML4, 0, 0x1000 );

	PageTableManager pageTableManager = PageTableManager( PML4 );

	for ( uint64_t t = 0; t < GetMemorySize( bootInfo->m_Map, m_MapEntries, bootInfo->m_MapDescriptorSize ); t += 0x1000 )
		pageTableManager.MapMemory( (void*)t, (void*)t );

	uint64_t fbBase = (uint64_t)bootInfo->framebuffer->BaseAddress;
	uint64_t fbSize = (uint64_t)bootInfo->framebuffer->BufferSize + 0x1000; // Add extra page

	for ( uint64_t t = fbBase; t < fbBase + fbSize; t += 0x1000 )
		pageTableManager.MapMemory( (void*)t, (void*)t );

	// Replace UEFI page table manager
	// clang-format off
	asm( "mov %0, %%cr3" : : "r"( PML4 ) ); // Put PLM4 into register 0, then execute command
	// clang-format on

	return;
}