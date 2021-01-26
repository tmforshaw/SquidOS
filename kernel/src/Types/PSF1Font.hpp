#pragma once
#include "IntDef.hpp"

struct PSF1_HEADER
{
	uint8_t magic[2]; // The bytes that the header stores to identify the file as a psf file
	uint8_t mode;
	uint8_t charsize; // Byte size of each character
};

extern uint8_t PSF1_FontWidth;

struct PSF1_FONT
{
	PSF1_HEADER* psf1_Header;
	void* glyphBuffer;
};