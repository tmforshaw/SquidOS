#include "EFI_Memory.hpp"

const char* EFI_MEMORY_TYPE_STRINGS[] {

	"EfiReservedMemoryType",
	"EfiLoaderCode",
	"EfiLoaderData",
	"EfiBootServicesCode",
	"EfiBootServicesData",
	"EfiRuntimeServicesCode",
	"EfiRuntimeServicesData",
	"EfiConventionalMemory", // Memory section that UEFI hasn't used (free to use memory)
	"EfiUnusableMemory",
	"EfiACPIReclaimMemory", // Memory that we can use, but we need information from the ACPI tables
	"EfiACPIMemoryNVS",
	"EfiMemoryMappedIO",
	"EfiMemoryMappedIOPortSpace",
	"EfiPalCode"
};