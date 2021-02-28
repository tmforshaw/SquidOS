#pragma once
#include "../Types/IntDef.hpp"

struct HeapSegHdr
{
	size_t		length;
	HeapSegHdr* next;
	HeapSegHdr* last;
	bool		free;

	void		CombineForward();
	void		CombineBackwards();
	HeapSegHdr* Split( size_t splitLength ); // Split length is the size of the first of the two new headers
};

void InitialiseHeap( void* heapAddress, size_t pageCount );

void* malloc( size_t size );
void  free( void* address );

void ExpandHeap( size_t extraPageCount );