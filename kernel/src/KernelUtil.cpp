#include "KernelUtil.hpp"

#include "GDT/GDT.hpp"
#include "IO/IO.hpp"
#include "Interrupts/IDT.hpp"
#include "Interrupts/Interrupts.hpp"
#include "Memory/Heap.hpp"
#include "PCI/PCI.hpp"

KernelInfo		 kernelInfo;
PageTableManager pageTableManager = nullptr;
IDTR			 idtr;

void PrepareMemory( BootInfo* bootInfo )
{
	uint64_t m_MapEntries = bootInfo->m_MapSize / bootInfo->m_MapDescriptorSize;

	GlobalAllocator = PageFrameAllocator(); // Instantiate
	GlobalAllocator.ReadEFIMemoryMap( bootInfo->m_Map, bootInfo->m_MapSize, bootInfo->m_MapDescriptorSize );

	// Lock the pages where the kernel is located
	uint64_t kernelSize	 = ( uint64_t )( &_KernelEnd ) - ( uint64_t )( &_KernelStart );
	uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;
	GlobalAllocator.LockPages( &_KernelStart, kernelPages );

	PageTable* PML4 = (PageTable*)GlobalAllocator.RequestPage();
	memset( PML4, 0, 0x1000 );

	GlobalPageTableManager = PageTableManager( PML4 );

	for ( uint64_t t = 0; t < GetMemorySize( bootInfo->m_Map, m_MapEntries, bootInfo->m_MapDescriptorSize ); t += 0x1000 )
		GlobalPageTableManager.MapMemory( (void*)t, (void*)t );

	uint64_t fbBase = (uint64_t)bootInfo->framebuffer->BaseAddress;
	uint64_t fbSize = (uint64_t)bootInfo->framebuffer->BufferSize + 0x1000; // Add extra page

	GlobalAllocator.LockPages( (void*)fbBase, fbSize / 0x1000 + 1 ); // Lock the frame buffer's pages

	for ( uint64_t t = fbBase; t < fbBase + fbSize; t += 0x1000 )
		GlobalPageTableManager.MapMemory( (void*)t, (void*)t );

	// Replace UEFI page table manager
	asm( "mov %0, %%cr3"
		 :
		 : "r"( PML4 ) ); // Put PLM4 into register 0, then execute command

	kernelInfo.pageTableManager = &GlobalPageTableManager;
}

void SetIDT_Gate( void* handler, uint8_t entryOffset, uint8_t type_attr, uint8_t selector )
{
	IDT_DescEntry* interrupt = (IDT_DescEntry*)( idtr.Offset + entryOffset * sizeof( IDT_DescEntry ) );
	interrupt->SetOffset( (uint64_t)handler );
	interrupt->Type_attr = type_attr;
	interrupt->Selector	 = selector;
}

void PrepareInterrupts()
{
	idtr.Limit	= 0x0FFF;
	idtr.Offset = (uint64_t)GlobalAllocator.RequestPage();

	SetIDT_Gate( (void*)PageFault_Handler, 0x0E, IDT_TA_InterruptGate, 0x08 );	 // Page fault
	SetIDT_Gate( (void*)DoubleFault_Handler, 0x08, IDT_TA_InterruptGate, 0x08 ); // Double fault
	SetIDT_Gate( (void*)GPFault_Handler, 0x0D, IDT_TA_InterruptGate, 0x08 );	 // General protection fault
	SetIDT_Gate( (void*)KeyboardInt_Handler, 0x21, IDT_TA_InterruptGate, 0x08 ); // Keyboard handler
	SetIDT_Gate( (void*)MouseInt_Handler, 0x2C, IDT_TA_InterruptGate, 0x08 );	 // Mouse handler

	asm( "lidt %0"
		 :
		 : "m"( idtr ) ); // Moves idtr and loads gdt

	RemapPIC();
}

void PrepareACPI( BootInfo* bootInfo )
{
	ACPI::SDT_Header* xsdt = (ACPI::SDT_Header*)( bootInfo->rsdp->XSDT_Address );

	ACPI::MCFG_Header* mcfg = (ACPI::MCFG_Header*)ACPI::FindTable( xsdt, (char*)"MCFG" );

	PCI::EnumeratePCI( mcfg );

	GlobalRenderer->Endl();
}

BasicRenderer ren = BasicRenderer( nullptr, nullptr );
SoundManager  sou = SoundManager();
KernelInfo	  InitialiseKernel( BootInfo* bootInfo )
{
	// Initialise Renderer
	ren			   = BasicRenderer( bootInfo->framebuffer, bootInfo->psf1_font );
	GlobalRenderer = &ren;

	// Initialise GDT
	GDT_Descriptor gdtDescriptor;
	gdtDescriptor.Size	 = sizeof( GDT ) - 1;
	gdtDescriptor.Offset = ( uint64_t )( &DefaultGDT );
	LoadGDT( &gdtDescriptor );

	// Initialise Memory
	PrepareMemory( bootInfo );

	// Clear framebuffer
	memset( bootInfo->framebuffer->BaseAddress, 0, bootInfo->framebuffer->BufferSize );

	// Initialise the heap
	InitialiseHeap( (void*)0x0000100000000000, 0x10 ); // Initialise with 10 pages

	// Initialise Interrupts
	PrepareInterrupts();

	// Initialise Mouse
	InitPS2Mouse();

	// Initialise ACPI
	PrepareACPI( bootInfo );

	// Unmask PIC DATA
	outb( PIC1_DATA, 0b11111001 );
	outb( PIC2_DATA, 0b11101111 );

	// Initialise Sound
	GlobalSound = &sou;

	// // Initialise Command Line Interface
	// GlobalCommand = CommandManager( { 250, 25 }, 300, 75 ); // Initialise Command Manager
	// SelectedTextUI = &GlobalCommand.GetTextUI();			// Select this text box

	asm( "sti" ); // Enable maskable interrupts

	// Display RAM data
	PrintMemoryDebug();
	GlobalRenderer->Endl();

	return kernelInfo;
}