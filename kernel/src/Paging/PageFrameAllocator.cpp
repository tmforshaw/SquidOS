#include "PageFrameAllocator.hpp"

#include "PageMapIndexer.hpp"

uint64_t freeMemory; // Free memory of the entire system
uint64_t reservedMemory;
uint64_t usedMemory; // Allocated memory
static bool Initialised = false;
PageFrameAllocator GlobalAllocator;
static uint64_t pageBitmapIndex = 0; // Index for requesting a page

void PageFrameAllocator::InitBitmap( size_t bitmapSize, void* bufferAddress )
{
	PageBitmap.Size = bitmapSize;
	PageBitmap.Buffer = (uint8_t*)bufferAddress;

	for ( int i = 0; i < bitmapSize; i++ )
		*(uint8_t*)( PageBitmap.Buffer + i ) = 0;
}

void PageFrameAllocator::FreePage( void* address )
{
	uint64_t index = (uint64_t)address / PAGE_SIZE;

	if ( PageBitmap[index] == false ) return;

	if ( PageBitmap.Set( index, false ) )
	{
		freeMemory += PAGE_SIZE;
		usedMemory -= PAGE_SIZE;

		// Change the pagingIndex
		if ( pageBitmapIndex > index ) pageBitmapIndex = index;
	}
}

void PageFrameAllocator::LockPage( void* address )
{
	uint64_t index = (uint64_t)address / PAGE_SIZE;

	if ( PageBitmap[index] == true ) return;

	if ( PageBitmap.Set( index, true ) )
	{
		freeMemory -= PAGE_SIZE;
		usedMemory += PAGE_SIZE;
	}
}

void PageFrameAllocator::UnreservePage( void* address )
{
	uint64_t index = (uint64_t)address / PAGE_SIZE;

	if ( PageBitmap[index] == false ) return;

	if ( PageBitmap.Set( index, false ) )
	{
		freeMemory += PAGE_SIZE;
		reservedMemory -= PAGE_SIZE;

		// Change the pagingIndex
		if ( pageBitmapIndex > index ) pageBitmapIndex = index;
	}
}

void PageFrameAllocator::ReservePage( void* address )
{
	uint64_t index = (uint64_t)address / PAGE_SIZE;

	if ( PageBitmap[index] == true ) return;

	if ( PageBitmap.Set( index, true ) )
	{
		freeMemory -= PAGE_SIZE;
		reservedMemory += PAGE_SIZE;
	}
}

void PageFrameAllocator::FreePages( void* address, uint64_t pageCount )
{
	for ( int t = 0; t < pageCount; t++ )
		FreePage( (void*)( (uint64_t)address + ( t * PAGE_SIZE ) ) );
}

void PageFrameAllocator::LockPages( void* address, uint64_t pageCount )
{
	for ( int t = 0; t < pageCount; t++ )
		LockPage( (void*)( (uint64_t)address + ( t * PAGE_SIZE ) ) );
}

void PageFrameAllocator::UnreservePages( void* address, uint64_t pageCount )
{
	for ( int t = 0; t < pageCount; t++ )
		UnreservePage( (void*)( (uint64_t)address + ( t * PAGE_SIZE ) ) );
}

void PageFrameAllocator::ReservePages( void* address, uint64_t pageCount )
{
	for ( int t = 0; t < pageCount; t++ )
		ReservePage( (void*)( (uint64_t)address + ( t * PAGE_SIZE ) ) );
}

void PageFrameAllocator::ReadEFIMemoryMap( EFI_MEMORY_DESCRIPTOR* m_Map, size_t m_MapSize, size_t m_MapDescriptorSize )
{
	if ( Initialised )
		return;

	Initialised = true;

	uint64_t m_MapEntries = m_MapSize / m_MapDescriptorSize;

	void* largestFreeMemSeg = nullptr;
	size_t largestFreeMemSegSize = 0;

	for ( int i = 0; i < m_MapEntries; i++ )
	{
		EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)( (uint64_t)m_Map + ( i * m_MapDescriptorSize ) );

		if ( desc->type == 7 ) // Type == EfiConventionalMemory
		{
			if ( desc->numPages * PAGE_SIZE > largestFreeMemSegSize )
			{
				largestFreeMemSeg = desc->physAddr;
				largestFreeMemSegSize = desc->numPages * PAGE_SIZE;
			}
		}
	}

	uint64_t memorySize = GetMemorySize( m_Map, m_MapEntries, m_MapDescriptorSize );
	freeMemory = memorySize;

	uint64_t bitmapSize = memorySize / PAGE_SIZE / 8 + 1; // +1 To ensure we have enough space

	InitBitmap( bitmapSize, largestFreeMemSeg );

	LockPages( PageBitmap.Buffer, PageBitmap.Size / PAGE_SIZE + 1 );

	for ( int i = 0; i < m_MapEntries; i++ )
	{
		EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)( (uint64_t)m_Map + ( i * m_MapDescriptorSize ) );

		if ( desc->type != 7 ) // Type != EfiConventionalMemory
			ReservePages( desc->physAddr, desc->numPages );
	}
}

void* PageFrameAllocator::RequestPage()
{

	for ( ; pageBitmapIndex < PageBitmap.Size * 8; pageBitmapIndex++ )
	{
		if ( PageBitmap[pageBitmapIndex] == true ) continue;

		LockPage( (void*)( pageBitmapIndex * PAGE_SIZE ) );

		return (void*)( pageBitmapIndex * PAGE_SIZE );
	}

	return nullptr; // Page Frame Swap to file
}

uint64_t PageFrameAllocator::GetFreeRAM() { return freeMemory; }
uint64_t PageFrameAllocator::GetUsedRAM() { return usedMemory; }
uint64_t PageFrameAllocator::GetReservedRAM() { return reservedMemory; }