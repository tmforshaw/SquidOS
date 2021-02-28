#include "Heap.hpp"

#include "../Paging/PageFrameAllocator.hpp"
#include "../Paging/PageTableManager.hpp"

void*		heapStart;
void*		heapEnd;
HeapSegHdr* lastHdr;

HeapSegHdr* HeapSegHdr::Split( size_t extraPageCount )
{
	if ( extraPageCount < 0x10 ) return nullptr;
	int64_t splitSegLength = length - extraPageCount - sizeof( HeapSegHdr );
	if ( splitSegLength < 0x10 ) return nullptr;

	HeapSegHdr* newSplitHdr = (HeapSegHdr*)( (size_t)this + extraPageCount + sizeof( HeapSegHdr ) );

	next->last			= newSplitHdr;
	newSplitHdr->next	= next;
	next				= newSplitHdr;
	newSplitHdr->last	= this;
	newSplitHdr->length = splitSegLength;
	newSplitHdr->free	= free;
	length				= extraPageCount;

	if ( lastHdr == this ) lastHdr = newSplitHdr;

	return newSplitHdr;
}

void HeapSegHdr::CombineForward()
{
	if ( next == nullptr || !next->free ) return;
	if ( next == lastHdr ) lastHdr = this;

	if ( next->next != nullptr )
		next->next->last = this;

	length += next->length + sizeof( HeapSegHdr );
}

void HeapSegHdr::CombineBackwards()
{
	if ( last != nullptr && last->free ) last->CombineForward();
}

void InitialiseHeap( void* heapAddress, size_t pageCount )
{
	void* pos = heapAddress;

	for ( size_t i = 0; i < pageCount; i++ )
	{
		GlobalPageTableManager.MapMemory( pos, GlobalAllocator.RequestPage() );
		pos = (void*)( (size_t)pos + 0x1000 ); // Increment the page
	}

	size_t heapLength = pageCount * 0x1000;

	heapStart = heapAddress;
	heapEnd	  = (void*)( (size_t)heapStart + heapLength );

	HeapSegHdr* startSeg = (HeapSegHdr*)heapAddress;
	startSeg->length	 = heapLength - sizeof( HeapSegHdr );
	startSeg->next		 = nullptr;
	startSeg->last		 = nullptr;
	startSeg->free		 = true;

	lastHdr = startSeg;
}

void* malloc( size_t size )
{
	// Round size upwards to a 64 bit number
	if ( size % 0x10 > 0 )						  // Not a multiple of 128
		size = ( size - ( size % 0x10 ) ) + 0x10; // Remove remainder then round up

	if ( size == 0 ) return nullptr;

	// Find a memory segment
	HeapSegHdr* currentSeg = (HeapSegHdr*)heapStart;
	while ( true ) // Iterate entire heap
	{
		if ( currentSeg->free )
		{
			if ( currentSeg->length > size )
			{
				currentSeg->Split( size );
				currentSeg->free = false;
				return (void*)( (uint64_t)currentSeg + sizeof( HeapSegHdr ) );
			}
			else if ( currentSeg->length == size )
			{
				currentSeg->free = false;
				return (void*)( (uint64_t)currentSeg + sizeof( HeapSegHdr ) );
			}
		}

		if ( currentSeg->next == nullptr ) break; // Only leave the loop if the next seg is null

		currentSeg = currentSeg->next;
	}

	ExpandHeap( size );	   // Expand the heap
	return malloc( size ); // Call malloc to allocate the new memory
}

void free( void* address )
{
	HeapSegHdr* segment = (HeapSegHdr*)address - 1; // Get the header not the block

	segment->free = true;
	segment->CombineForward();
	segment->CombineBackwards();
}

void ExpandHeap( size_t extraPageCount )
{
	if ( extraPageCount % 0x1000 > 0 )
		extraPageCount = ( extraPageCount - ( extraPageCount % 0x1000 ) ) + 0x1000; // Remove remainder then round up

	size_t pageCount = extraPageCount / 0x1000;

	HeapSegHdr* newSegment = (HeapSegHdr*)heapEnd;

	for ( size_t i = 0; i < pageCount; i++ )
	{
		GlobalPageTableManager.MapMemory( heapEnd, GlobalAllocator.RequestPage() );
		heapEnd = (void*)( (size_t)heapEnd + 0x1000 ); // Increment heapEnd
	}

	newSegment->free   = true;
	newSegment->last   = lastHdr;
	lastHdr->next	   = newSegment;
	lastHdr			   = newSegment;
	newSegment->next   = nullptr;
	newSegment->length = extraPageCount - sizeof( HeapSegHdr );
	newSegment->CombineBackwards();
}
