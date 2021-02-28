#pragma once
#include "Paging.hpp"

class PageTableManager
{
public:
	PageTable* PML4;

	PageTableManager( PageTable* PML4Address );
	void MapMemory( void* virtualMemory, void* physicalMemory );
};

extern PageTableManager GlobalPageTableManager;
