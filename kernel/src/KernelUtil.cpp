#include "KernelUtil.hpp"

KernelInfo kernelInfo;
PageTableManager pageTableManager = NULL;

void PrepareMemory( BootInfo* bootInfo )
{
	uint64_t m_MapEntries = bootInfo->m_MapSize / bootInfo->m_MapDescriptorSize;

	GlobalAllocator = PageFrameAllocator(); // Instantiate
	GlobalAllocator.ReadEFIMemoryMap( bootInfo->m_Map, bootInfo->m_MapSize, bootInfo->m_MapDescriptorSize );

	// Lock the pages where the kernel is located
	uint64_t kernelSize = ( uint64_t )( &_KernelEnd ) - ( uint64_t )( &_KernelStart );
	uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;
	GlobalAllocator.LockPages( &_KernelStart, kernelPages );

	PageTable* PML4 = (PageTable*)GlobalAllocator.RequestPage();
	memset( PML4, 0, 0x1000 );

	pageTableManager = PageTableManager( PML4 );

	for ( uint64_t t = 0; t < GetMemorySize( bootInfo->m_Map, m_MapEntries, bootInfo->m_MapDescriptorSize ); t += 0x1000 )
		pageTableManager.MapMemory( (void*)t, (void*)t );

	uint64_t fbBase = (uint64_t)bootInfo->framebuffer->BaseAddress;
	uint64_t fbSize = (uint64_t)bootInfo->framebuffer->BufferSize + 0x1000; // Add extra page

	GlobalAllocator.LockPages( (void*)fbBase, fbSize / 0x1000 + 1 ); // Lock the frame buffer's pages

	for ( uint64_t t = fbBase; t < fbBase + fbSize; t += 0x1000 )
		pageTableManager.MapMemory( (void*)t, (void*)t );

	// Replace UEFI page table manager
	asm( "mov %0, %%cr3"
		 :
		 : "r"( PML4 ) ); // Put PLM4 into register 0, then execute command

	memset( bootInfo->framebuffer->BaseAddress, 0, bootInfo->framebuffer->BufferSize ); // Set frame buffer to black

	kernelInfo.pageTableManager = &pageTableManager;
}

KernelInfo InitialiseKernel( BootInfo* bootInfo )
{
	PrepareMemory( bootInfo );

	return kernelInfo;
}