#include <efi.h>
#include <efilib.h>
#include <elf.h>

typedef unsigned long long size_t;

EFI_FILE* LoadFile(EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	EFI_FILE* LoadedFile;

	// Get the Loaded Image Protocol
	EFI_LOADED_IMAGE_PROTOCOL* LoadedImage;
	SystemTable->BootServices->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (void**)(&LoadedImage));

	// Get the file system that we booted from
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;
	SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)(&FileSystem));

	// If no directory is specified, set directory to root of filesystem
	if (Directory == NULL)
		FileSystem->OpenVolume(FileSystem, &Directory);

	// Opens directory then saves the file into LoadedFile
	EFI_STATUS s = Directory->Open(Directory, &LoadedFile, Path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);

	if (s != EFI_SUCCESS)
		return NULL;

	return LoadedFile;
}

int memcmp(const void* aptr, const void* bptr, size_t n)
{
	const unsigned char *a = aptr, *b = bptr;
	for (size_t i = 0; i < n; i++)
	{
		if (a[i] < b[i])
			return -1;
		else if (a[i] > b[i])
			return 1;
	}

	return 0;
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	InitializeLib(ImageHandle, SystemTable);
	Print(L"String Blah Blah\n\r");

	EFI_FILE* Kernel = LoadFile(NULL, L"kernel.elf", ImageHandle, SystemTable);

	if (Kernel == NULL)
		Print(L"Couldn't load kernel.\n\r");
	else
		Print(L"Kernel loaded successfully.\n\r");

	Elf64_Ehdr header;
	{
		UINTN FileInfoSize;
		EFI_FILE_INFO* FileInfo;
		Kernel->GetInfo(Kernel, &gEfiFileInfoGuid, &FileInfoSize, NULL); // Set FileInfoSize to size that we need for this kernel
		SystemTable->BootServices->AllocatePool(EfiLoaderData, FileInfoSize, (void**)(&FileInfo));
		Kernel->GetInfo(Kernel, &gEfiFileInfoGuid, &FileInfoSize, (void**)(&FileInfo));

		UINTN size = sizeof(header);
		Kernel->Read(Kernel, &size, &header);
	}

	// clang-format off

	if(
		memcmp(&header.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0 || 
		header.e_ident[EI_CLASS] != ELFCLASS64 ||
		header.e_ident[EI_DATA] != ELFDATA2LSB ||
		header.e_type != ET_EXEC ||
		header.e_machine != EM_X86_64 ||
		header.e_version != EV_CURRENT
	)
	{
		Print(L"Kernel format is bad.\n\r");
		
		// Shady debugging code

		Print(L"\n\r");

		if((memcmp(&header.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0)) Print(L"1"); // Returns false
		else Print(L"0");

		if(header.e_ident[EI_CLASS] != ELFCLASS64) Print(L"1"); // Returns false
		else Print(L"0");
		
		if(header.e_ident[EI_DATA] != ELFDATA2LSB) Print(L"1"); // Returns false
		else Print(L"0");

		if(header.e_type != ET_EXEC) Print(L"1"); // Returns true
		else Print(L"0");

		if(header.e_machine != EM_X86_64) Print(L"1"); // Returns false
		else Print(L"0");

		if(header.e_version != EV_CURRENT) Print(L"1"); // Returns false
		else Print(L"0");
		
		Print(L"\n\r");
	}
	else
	{
		Print(L"Kernel header successfully verified.\n\r");
	}

	// clang-format on

	return EFI_SUCCESS; // Exit the UEFI application
}
