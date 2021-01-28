#pragma once
#include "../Types/IntDef.hpp"

#define PAGE_SIZE 4096

class PageMapIndexer
{
public:
	uint64_t PDP_i; // Page Directory Pointer index
	uint64_t PD_i;	// Page Directory index
	uint64_t PT_i;	// Page Table index
	uint64_t P_i;	// Page index

	PageMapIndexer( uint64_t virtualAddress );
};