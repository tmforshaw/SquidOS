#include "PageMapIndexer.hpp"

PageMapIndexer::PageMapIndexer( uint64_t virtualAddress )
{
	virtualAddress >>= 12; // All addresses in each page are 12-bit aligned (4096 bytes)
	P_i = virtualAddress & 0x1FF;
	virtualAddress >>= 9;
	PT_i = virtualAddress & 0x1FF;
	virtualAddress >>= 9;
	PD_i = virtualAddress & 0x1FF;
	virtualAddress >>= 9;
	PDP_i = virtualAddress & 0x1FF;
}