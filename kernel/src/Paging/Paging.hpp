#pragma once
#include <stdint.h>

struct PageDirectoryEntry
{
	bool Present : 1;	// Exists
	bool ReadWrite : 1; // You can read or write
	bool UserSuper : 1; // Accessable by user or an admin
	bool WriteThrough : 1;
	bool CacheDisabled : 1;
	bool Accesssed : 1;	  // Set to true whenever the CPU accesses this page in memory
	bool Ignore0 : 1;	  // Ignore (Unusable bit)
	bool LargerPages : 1; // Turns child page directory into a page (with the same size it would have had)
	bool Ignore1 : 1;
	uint8_t Available : 3; // Can be used for anything
	uint64_t Address : 52;
};

struct PageTable
{
	PageDirectoryEntry entries[512];
} __attribute__( ( aligned( 0x1000 ) ) );