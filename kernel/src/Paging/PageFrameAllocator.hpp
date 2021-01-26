#pragma once
#include "../Memory/EFI_Memory.hpp"
#include "../Memory/Memory.hpp"
#include "../Types/Bitmap.hpp"
#include "../Types/IntDef.hpp"

class PageFrameAllocator
{
private:
	void InitBitmap( size_t bitmapSize, void* bufferAddress );

	void UnreservePage( void* address );
	void ReservePage( void* address );

	void UnreservePages( void* address, uint64_t pageCount );
	void ReservePages( void* address, uint64_t pageCount );

public:
	Bitmap PageBitmap;

	void FreePage( void* address );
	void LockPage( void* address );

	void FreePages( void* address, uint64_t pageCount );
	void LockPages( void* address, uint64_t pageCount );

	void ReadEFIMemoryMap( EFI_MEMORY_DESCRIPTOR* m_Map, size_t m_MapSize, size_t m_MapDescriptorSize );

	void* RequestPage();

	// Getters
	uint64_t GetFreeRAM();
	uint64_t GetUsedRAM();
	uint64_t GetReservedRAM();
};

extern PageFrameAllocator GlobalAllocator;