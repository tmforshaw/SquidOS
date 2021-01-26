#include "KernelUtil.hpp"

#include "GDT/GDT.hpp"
#include "IO/IO.hpp"
#include "Interrupts/IDT.hpp"
#include "Interrupts/Interrupts.hpp"

KernelInfo kernelInfo;
PageTableManager pageTableManager = nullptr;
IDTR idtr;

void PrepareMemory( BootInfo* bootInfo )
{
	uint64_t m_MapEntries = bootInfo->m_MapSize / bootInfo->m_MapDescriptorSize;

	GlobalAllocator = PageFrameAllocator(); // Instantiate
	GlobalAllocator.ReadEFIMemoryMap( bootInfo->m_Map, bootInfo->m_MapSize, bootInfo->m_MapDescriptorSize );

	// Lock the pages where the kernel is located
	uint64_t kernelSize = ( uint64_t )( &_KernelEnd ) - ( uint64_t )( &_KernelStart );
	uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;
	GlobalAllocator.LockPages( &_KernelStart, kernelPages );

	PageTable* PML4 = (PageTable*)GlobalAllocator.RequestPage();
	memset( PML4, 0, 0x1000 );

	pageTableManager = PageTableManager( PML4 );

	for ( uint64_t t = 0; t < GetMemorySize( bootInfo->m_Map, m_MapEntries, bootInfo->m_MapDescriptorSize ); t += 0x1000 )
		pageTableManager.MapMemory( (void*)t, (void*)t );

	uint64_t fbBase = (uint64_t)bootInfo->framebuffer->BaseAddress;
	uint64_t fbSize = (uint64_t)bootInfo->framebuffer->BufferSize + 0x1000; // Add extra page

	GlobalAllocator.LockPages( (void*)fbBase, fbSize / 0x1000 + 1 ); // Lock the frame buffer's pages

	for ( uint64_t t = fbBase; t < fbBase + fbSize; t += 0x1000 )
		pageTableManager.MapMemory( (void*)t, (void*)t );

	// Replace UEFI page table manager
	asm( "mov %0, %%cr3"
		 :
		 : "r"( PML4 ) ); // Put PLM4 into register 0, then execute command

	kernelInfo.pageTableManager = &pageTableManager;
}

void PrepareInterrupts()
{
	idtr.Limit = 0x0FFF;
	idtr.Offset = (uint64_t)GlobalAllocator.RequestPage();

	// Page fault
	IDT_DescEntry* int_PageFault = (IDT_DescEntry*)( idtr.Offset + 0xE * sizeof( IDT_DescEntry ) );
	int_PageFault->SetOffset( (uint64_t)PageFault_Handler );
	int_PageFault->Type_attr = IDT_TA_InterruptGate;
	int_PageFault->Selector = 0x08;

	// Double fault
	IDT_DescEntry* int_DoubleFault = (IDT_DescEntry*)( idtr.Offset + 0x8 * sizeof( IDT_DescEntry ) );
	int_DoubleFault->SetOffset( (uint64_t)DoubleFault_Handler );
	int_DoubleFault->Type_attr = IDT_TA_InterruptGate;
	int_DoubleFault->Selector = 0x08;

	// General protection fault
	IDT_DescEntry* int_GPFault = (IDT_DescEntry*)( idtr.Offset + 0xD * sizeof( IDT_DescEntry ) );
	int_GPFault->SetOffset( (uint64_t)GPFault_Handler );
	int_GPFault->Type_attr = IDT_TA_InterruptGate;
	int_GPFault->Selector = 0x08;

	// Keyboard handler
	IDT_DescEntry* int_Keyboard = (IDT_DescEntry*)( idtr.Offset + 0x21 * sizeof( IDT_DescEntry ) );
	int_Keyboard->SetOffset( (uint64_t)KeyboardInt_Handler );
	int_Keyboard->Type_attr = IDT_TA_InterruptGate;
	int_Keyboard->Selector = 0x08;

	asm( "lidt %0"
		 :
		 : "m"( idtr ) ); // Moves idtr and loads gdt

	RemapPIC();

	outb( PIC1_DATA, 0b11111101 ); // Unmask second interrupt
	outb( PIC2_DATA, 0b11111111 );

	asm( "sti" ); // Enable maskable interrupts
}

BasicRenderer r = BasicRenderer( nullptr, nullptr );
KernelInfo InitialiseKernel( BootInfo* bootInfo )
{
	r = BasicRenderer( bootInfo->framebuffer, bootInfo->psf1_font );
	GlobalRenderer = &r;

	GDT_Descriptor gdtDescriptor;
	gdtDescriptor.Size = sizeof( GDT ) - 1;
	gdtDescriptor.Offset = ( uint64_t )( &DefaultGDT );
	LoadGDT( &gdtDescriptor );

	PrepareMemory( bootInfo );

	memset( bootInfo->framebuffer->BaseAddress, 0, bootInfo->framebuffer->BufferSize ); // Set frame buffer to black

	PrepareInterrupts();

	return kernelInfo;
}