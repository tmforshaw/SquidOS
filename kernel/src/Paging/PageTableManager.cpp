#include "PageTableManager.hpp"

#include "../Memory/Memory.hpp"
#include "../Types/IntDef.hpp"
#include "PageFrameAllocator.hpp"
#include "PageMapIndexer.hpp"

PageTableManager::PageTableManager( PageTable* PML4Address )
{
	this->PML4 = PML4Address;
}

void PageTableManager::MapMemory( void* virtualMemory, void* physicalMemory )
{
	PageMapIndexer indexer = PageMapIndexer( (uint64_t)virtualMemory );
	PageDirectoryEntry PDE;

	PDE = PML4->entries[indexer.PDP_i];
	PageTable* PDP;

	if ( !PDE.GetFlag( PT_Flag::Present ) ) // PDP not present
	{
		PDP = (PageTable*)GlobalAllocator.RequestPage();
		memset( PDP, 0, PAGE_SIZE );
		PDE.SetAddress( (uint64_t)PDP >> 12 );
		PDE.SetFlag( PT_Flag::Present, true ); // Exists now
		PDE.SetFlag( PT_Flag::ReadWrite, true );
		PML4->entries[indexer.PDP_i] = PDE;
	}
	else
	{
		PDP = (PageTable*)( (uint64_t)PDE.GetAddress() << 12 );
	}

	PDE = PDP->entries[indexer.PD_i];
	PageTable* PD;

	if ( !PDE.GetFlag( PT_Flag::Present ) ) // PD not present
	{
		PD = (PageTable*)GlobalAllocator.RequestPage();
		memset( PD, 0, PAGE_SIZE );
		PDE.SetAddress( (uint64_t)PD >> 12 );
		PDE.SetFlag( PT_Flag::Present, true ); // Exists now
		PDE.SetFlag( PT_Flag::ReadWrite, true );
		PDP->entries[indexer.PD_i] = PDE;
	}
	else
	{
		PD = (PageTable*)( (uint64_t)PDE.GetAddress() << 12 );
	}

	PDE = PD->entries[indexer.PT_i];
	PageTable* PT;

	if ( !PDE.GetFlag( PT_Flag::Present ) ) // PT not present
	{
		PT = (PageTable*)GlobalAllocator.RequestPage();
		memset( PT, 0, PAGE_SIZE );
		PDE.SetAddress( (uint64_t)PT >> 12 );
		PDE.SetFlag( PT_Flag::Present, true ); // Exists now
		PDE.SetFlag( PT_Flag::ReadWrite, true );
		PD->entries[indexer.PT_i] = PDE;
	}
	else
	{
		PT = (PageTable*)( (uint64_t)PDE.GetAddress() << 12 );
	}

	PDE = PT->entries[indexer.P_i];
	PDE.SetAddress( (uint64_t)physicalMemory >> 12 );
	PDE.SetFlag( PT_Flag::Present, true ); // Exists now
	PDE.SetFlag( PT_Flag::ReadWrite, true );
	PT->entries[indexer.P_i] = PDE;
}