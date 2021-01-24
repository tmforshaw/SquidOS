#pragma once

struct PSF1_HEADER
{
	unsigned char magic[2]; // The bytes that the header stores to identify the file as a psf file
	unsigned char mode;
	unsigned char charsize; // Byte size of each character
};

struct PSF1_FONT
{
	PSF1_HEADER* psf1_Header;
	void* glyphBuffer;
};