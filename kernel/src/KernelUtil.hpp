#pragma once

#include "IO/Mouse.hpp"
#include "Interfaces/CommandManager.hpp"
#include "Interfaces/TextInterface.hpp"
#include "Memory/EFI_Memory.hpp"
#include "Memory/Memory.hpp"
#include "PCI/ACPI.hpp"
#include "Paging/PageFrameAllocator.hpp"
#include "Paging/PageMapIndexer.hpp"
#include "Paging/PageTableManager.hpp"
#include "Paging/Paging.hpp"
#include "Rendering/BasicRenderer.hpp"
#include "Sound/Sound.hpp"
#include "Types/Bitmap.hpp"
#include "Types/C_String.hpp"
#include "Types/String.hpp"

struct BootInfo
{
	Framebuffer* framebuffer;
	PSF1_FONT* psf1_font;
	EFI_MEMORY_DESCRIPTOR* m_Map;
	uint64_t m_MapSize;
	uint64_t m_MapDescriptorSize;
	ACPI::RSDP2* rsdp; // Root system descriptor pointer
};

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

struct KernelInfo
{
	PageTableManager* pageTableManager;
};

KernelInfo InitialiseKernel( BootInfo* bootInfo );