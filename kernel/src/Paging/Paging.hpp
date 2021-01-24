#pragma once
#include <stdint.h>

enum PT_Flag
{
	Present = 0,   // Exists
	ReadWrite = 1, // You can read or write
	UserSuper = 2, // Accessable by user or an admin
	WriteThrough = 3,
	CacheDisabled = 4,
	Accesssed = 5,	 // Set to true whenever the CPU accesses this page in memory
	LargerPages = 7, // Turns child page directory into a page (with the same size it would have had),
	Custom0 = 9,
	Custom1 = 10,
	Custom2 = 11,
	NX = 63 // Not executable bit (Only if supported)
};

struct PageDirectoryEntry
{
	uint64_t Value;

	void SetFlag( PT_Flag flag, bool enabled );
	bool GetFlag( PT_Flag flag );

	void SetAddress( uint64_t address );
	uint64_t GetAddress();
};

struct PageTable
{
	PageDirectoryEntry entries[512];
} __attribute__( ( aligned( 0x1000 ) ) );