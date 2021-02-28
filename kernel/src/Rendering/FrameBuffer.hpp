#pragma once

#include "../Types/IntDef.hpp" // For size_t

struct Framebuffer
{
	void* BaseAddress;
	size_t BufferSize;
	unsigned int Width;
	unsigned int Height;
	unsigned int PixelsPerScanLine;
};