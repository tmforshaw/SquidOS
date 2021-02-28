#include "PCI.hpp"

#include "../Paging/PageTableManager.hpp"
#include "../Rendering/BasicRenderer.hpp"
#include "../Types/C_String.hpp"

namespace PCI
{
	void EnumerateFunction( uint64_t deviceAddress, uint64_t function )
	{
		// Get offset of bus address from base address then add baseAddress
		uint64_t functionAddress = deviceAddress + ( function << 12 );

		GlobalPageTableManager.MapMemory( (void*)functionAddress, (void*)functionAddress );

		PCI_DeviceHeader* pciDeviceHeader = (PCI_DeviceHeader*)functionAddress;

		if ( pciDeviceHeader->DeviceID == 0 || pciDeviceHeader->DeviceID == 0xFFFF ) return; // Invalid

		GlobalRenderer->Print( GetVendorName( pciDeviceHeader->VendorID ) );
		GlobalRenderer->Print( " / " );
		GlobalRenderer->Print( GetDeviceName( pciDeviceHeader->VendorID, pciDeviceHeader->DeviceID ) );
		GlobalRenderer->Print( " / " );
		GlobalRenderer->Print( DeviceClasses[pciDeviceHeader->Class] );
		GlobalRenderer->Print( " / " );
		GlobalRenderer->Print( GetSubclassName( pciDeviceHeader->Class, pciDeviceHeader->Subclass ) );
		GlobalRenderer->Print( " / " );
		GlobalRenderer->Print( GetProgIF_Name( pciDeviceHeader->Class, pciDeviceHeader->Subclass, pciDeviceHeader->ProgIF ) );
		GlobalRenderer->Endl();
	}

	void EnumerateDevice( uint64_t busAddress, uint64_t device )
	{
		// Get offset of bus address from base address then add baseAddress
		uint64_t deviceAddress = busAddress + ( device << 15 );

		GlobalPageTableManager.MapMemory( (void*)deviceAddress, (void*)deviceAddress );

		PCI_DeviceHeader* pciDeviceHeader = (PCI_DeviceHeader*)deviceAddress;

		if ( pciDeviceHeader->DeviceID == 0 || pciDeviceHeader->DeviceID == 0xFFFF ) return; // Invalid

		for ( uint64_t function = 0; function < 8; function++ )
			EnumerateFunction( deviceAddress, function );
	}

	void EnumerateBus( uint64_t baseAddress, uint64_t bus )
	{
		// Get offset of bus address from base address then add baseAddress
		uint64_t busAddress = baseAddress + ( bus << 20 );

		GlobalPageTableManager.MapMemory( (void*)busAddress, (void*)busAddress );

		PCI_DeviceHeader* pciDeviceHeader = (PCI_DeviceHeader*)busAddress;

		if ( pciDeviceHeader->DeviceID == 0 || pciDeviceHeader->DeviceID == 0xFFFF ) return; // Invalid

		for ( uint64_t device = 0; device < 32; device++ )
			EnumerateDevice( busAddress, device );
	}

	void EnumeratePCI( ACPI::MCFG_Header* mcfg )
	{
		uint32_t entriesLength = ( ( mcfg->Header.Length ) - sizeof( ACPI::MCFG_Header ) ) / sizeof( ACPI::DeviceConfig ); // Amount of device configurations

		for ( uint16_t i = 0; i < entriesLength; i++ )
		{
			// Get base address of mcfg, add sizeof(mcfg) to get the end add sizeof(deviceconfig)*i to get base address of device configuration
			ACPI::DeviceConfig* newDeviceConfig = (ACPI::DeviceConfig*)( (uint64_t)mcfg + sizeof( ACPI::MCFG_Header ) + ( sizeof( ACPI::DeviceConfig ) * i ) );

			for ( uint64_t bus = newDeviceConfig->StartBus; bus < newDeviceConfig->EndBus; bus++ )
			{
				EnumerateBus( newDeviceConfig->BaseAddress, bus );
			}
		}
	}

} // namespace PCI