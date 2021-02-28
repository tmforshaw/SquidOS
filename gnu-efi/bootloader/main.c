#include <efi.h>
#include <efilib.h>
#include <elf.h>

typedef unsigned long long size_t;

typedef struct
{
	void* BaseAddress;
	size_t BufferSize;
	unsigned int Width;
	unsigned int Height;
	unsigned int PixelsPerScanLine;
} Framebuffer;

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

typedef struct
{
	unsigned char magic[2]; // The bytes that the header stores to identify the file as a psf file
	unsigned char mode;
	unsigned char charsize; // Byte size of each character
} PSF1_HEADER;

typedef struct
{
	PSF1_HEADER* psf1_Header;
	void* glyphBuffer;
} PSF1_FONT;

Framebuffer framebuffer;
Framebuffer* InitialiseGOP()
{
	EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
	EFI_STATUS status;

	status = uefi_call_wrapper( BS->LocateProtocol, 3, &gopGuid, NULL, (void**)( &gop ) ); // GNU_EFI feature that fixes calling convention for UEFI

	if ( EFI_ERROR( status ) )
	{
		Print( L"Unable to locate GOP.\n\r" );
		return NULL;
	}
	else
	{
		Print( L"GOP located.\n\r" );
	}

	framebuffer.BaseAddress = (void*)gop->Mode->FrameBufferBase;
	framebuffer.BufferSize = gop->Mode->FrameBufferSize;
	framebuffer.Width = gop->Mode->Info->HorizontalResolution;
	framebuffer.Height = gop->Mode->Info->VerticalResolution;
	framebuffer.PixelsPerScanLine = gop->Mode->Info->PixelsPerScanLine;

	return &framebuffer;
}

EFI_FILE* LoadFile( EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable )
{
	EFI_FILE* LoadedFile;

	// Get the Loaded Image Protocol
	EFI_LOADED_IMAGE_PROTOCOL* LoadedImage;
	SystemTable->BootServices->HandleProtocol( ImageHandle, &gEfiLoadedImageProtocolGuid, (void**)( &LoadedImage ) );

	// Get the file system that we booted from
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;
	SystemTable->BootServices->HandleProtocol( LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)( &FileSystem ) );

	// If no directory is specified, set directory to root of filesystem
	if ( Directory == NULL )
		FileSystem->OpenVolume( FileSystem, &Directory );

	// Opens directory then saves the file into LoadedFile
	EFI_STATUS s = Directory->Open( Directory, &LoadedFile, Path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY );

	if ( s != EFI_SUCCESS )
		return NULL;

	return LoadedFile;
}

PSF1_FONT* LoadPSF1Font( EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable )
{
	EFI_FILE* font = LoadFile( Directory, Path, ImageHandle, SystemTable );

	if ( font == NULL )
		return NULL;

	PSF1_HEADER* fontHeader;
	SystemTable->BootServices->AllocatePool( EfiLoaderData, sizeof( PSF1_HEADER ), (void**)( &fontHeader ) );
	UINTN size = sizeof( PSF1_HEADER );
	font->Read( font, &size, fontHeader );

	if ( fontHeader->magic[0] != PSF1_MAGIC0 || fontHeader->magic[1] != PSF1_MAGIC1 )
		return NULL;

	UINTN glyphBufferSize = fontHeader->charsize * 256;
	if ( fontHeader->mode == 1 ) // Has 512 glyphs
		glyphBufferSize *= 2;

	void* glyphBuffer;
	{
		font->SetPosition( font, sizeof( PSF1_HEADER ) );
		SystemTable->BootServices->AllocatePool( EfiLoaderData, glyphBufferSize, (void**)( &glyphBuffer ) );
		font->Read( font, &glyphBufferSize, glyphBuffer );
	}

	PSF1_FONT* finishedFont;
	SystemTable->BootServices->AllocatePool( EfiLoaderData, sizeof( PSF1_FONT ), (void**)( &finishedFont ) );
	finishedFont->psf1_Header = fontHeader;
	finishedFont->glyphBuffer = glyphBuffer;

	return finishedFont;
}

int memcmp( const void* aptr, const void* bptr, size_t n )
{
	const unsigned char *a = aptr, *b = bptr;
	for ( size_t i = 0; i < n; i++ )
	{
		if ( a[i] < b[i] )
			return -1;
		else if ( a[i] > b[i] )
			return 1;
	}

	return 0;
}

typedef struct
{
	Framebuffer* framebuffer;
	PSF1_FONT* psf1_font;
	EFI_MEMORY_DESCRIPTOR* m_Map;
	UINTN m_MapSize;
	UINTN m_MapDescriptorSize;
	void* rsdp; // Root system descriptor pointer
} BootInfo;

UINTN strcmp( CHAR8* a, CHAR8* b, UINTN length )
{
	for ( UINTN i = 0; i < length; i++ )
		if ( *a != *b ) return 0;

	return 1;
}

EFI_STATUS efi_main( EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable )
{
	InitializeLib( ImageHandle, SystemTable );
	Print( L"Welcome to SquidOS\n\r" );

	EFI_FILE* Kernel = LoadFile( NULL, L"kernel.elf", ImageHandle, SystemTable );

	if ( Kernel == NULL )
		Print( L"Couldn't load kernel.\n\r" );
	else
		Print( L"Kernel loaded successfully.\n\r" );

	Elf64_Ehdr header;
	{
		UINTN FileInfoSize;
		EFI_FILE_INFO* FileInfo;
		Kernel->GetInfo( Kernel, &gEfiFileInfoGuid, &FileInfoSize, NULL ); // Set FileInfoSize to size that we need for this kernel
		SystemTable->BootServices->AllocatePool( EfiLoaderData, FileInfoSize, (void**)( &FileInfo ) );
		Kernel->GetInfo( Kernel, &gEfiFileInfoGuid, &FileInfoSize, (void**)( &FileInfo ) );

		UINTN size = sizeof( header );
		Kernel->Read( Kernel, &size, &header );
	}

	if (
		memcmp( &header.e_ident[EI_MAG0], ELFMAG, SELFMAG ) != 0 ||
		header.e_ident[EI_CLASS] != ELFCLASS64 ||
		header.e_ident[EI_DATA] != ELFDATA2LSB ||
		header.e_type != ET_EXEC ||
		header.e_machine != EM_X86_64 ||
		header.e_version != EV_CURRENT )
	{
		Print( L"Kernel format is bad.\n\r" );
	}
	else
	{
		Print( L"Kernel header successfully verified.\n\r" );
	}

	Elf64_Phdr* phdrs;
	{
		Kernel->SetPosition( Kernel, header.e_phoff );
		UINTN size = header.e_phnum * header.e_phentsize;
		SystemTable->BootServices->AllocatePool( EfiLoaderData, size, (void**)( &phdrs ) );
		Kernel->Read( Kernel, &size, phdrs );
	}

	for (
		Elf64_Phdr* phdr = phdrs;
		(char*)phdr < (char*)phdrs + header.e_phnum * header.e_phentsize;
		phdr = (Elf64_Phdr*)( (char*)phdr + header.e_phentsize ) )
	{
		switch ( phdr->p_type )
		{
		case PT_LOAD: // Load Pages
		{
			int pages = ( phdr->p_memsz + 0x1000 - 1 ) / 0x1000; // Memory size and round it up
			Elf64_Addr segment = phdr->p_paddr;
			SystemTable->BootServices->AllocatePages( AllocateAddress, EfiLoaderData, pages, &segment );

			Kernel->SetPosition( Kernel, phdr->p_offset );
			UINTN size = phdr->p_filesz;
			Kernel->Read( Kernel, &size, (void*)segment );
			break;
		}
		}
	}

	Print( L"Kernel loaded.\n\r" );

	PSF1_FONT* newFont = LoadPSF1Font( NULL, L"zap-light16.psf", ImageHandle, SystemTable );
	if ( newFont == NULL )
		Print( L"New font is not valid, or is not found.\n\r" );
	else
		Print( L"Font found. Char size: %d.\n\r", newFont->psf1_Header->charsize );

	Framebuffer* newBuffer = InitialiseGOP();

	Print(
		L"Base: 0x%x\n\r\
		Size: 0x%x\n\r\
		Width: %d\n\r\
		Height: %d\n\r\
		PixelsPerScanLine: %d\n\r",
		newBuffer->BaseAddress,
		newBuffer->BufferSize,
		newBuffer->Width,
		newBuffer->Height,
		newBuffer->PixelsPerScanLine );

	EFI_MEMORY_DESCRIPTOR* Map = NULL;
	UINTN MapSize, MapKey;
	UINTN DescriptorSize;
	UINT32 DescriptorVersion;
	{
		SystemTable->BootServices->GetMemoryMap( &MapSize, Map, &MapKey, &DescriptorSize, &DescriptorVersion );
		SystemTable->BootServices->AllocatePool( EfiLoaderData, MapSize, (void**)( &Map ) );
		SystemTable->BootServices->GetMemoryMap( &MapSize, Map, &MapKey, &DescriptorSize, &DescriptorVersion );
	}

	EFI_CONFIGURATION_TABLE* configTable = SystemTable->ConfigurationTable;
	void* rsdp = NULL;

	// Search for ACPI 2.0 rsdp
	EFI_GUID ACPI2TableGUID = ACPI_20_TABLE_GUID;

	for ( UINTN i = 0; i < SystemTable->NumberOfTableEntries; i++ )
	{
		if ( CompareGuid( &configTable[i].VendorGuid, &ACPI2TableGUID ) ) // Found ACPI 2 Table
		{
			if ( strcmp( (CHAR8*)"RSD PTR ", (CHAR8*)configTable->VendorTable, 8 ) )
			{
				rsdp = (void*)configTable->VendorTable;
			}
		}

		configTable++;
	}

	void ( *KernelStart )( BootInfo* ) = ( (__attribute__( ( sysv_abi ) ) void ( * )( BootInfo* ))header.e_entry ); // Defines integer function ptr at correct address and assigns it attrib that allows correct function calling

	BootInfo bootInfo;
	bootInfo.framebuffer = newBuffer;
	bootInfo.psf1_font = newFont;
	bootInfo.m_Map = Map;
	bootInfo.m_MapSize = MapSize;
	bootInfo.m_MapDescriptorSize = DescriptorSize;
	bootInfo.rsdp = rsdp;

	Print( L"Exiting boot services.\n\r" );
	SystemTable->BootServices->ExitBootServices( ImageHandle, MapKey );

	Print( L"\n\rStarting kernel.\n\r" );
	KernelStart( &bootInfo );

	return EFI_SUCCESS; // Exit the UEFI application
}
