#pragma once
#include "../Types/IntDef.hpp"
#include "ACPI.hpp"

namespace PCI
{
	struct PCI_DeviceHeader
	{
		uint16_t VendorID; // ID of the device manufacturer
		uint16_t DeviceID;
		uint16_t Command;
		uint16_t Status;
		uint8_t	 RevisionID;
		uint8_t	 ProgIF; // Program interface
		uint8_t	 Subclass;
		uint8_t	 Class;
		uint8_t	 CacheLineSize;
		uint8_t	 LatencyTimer;
		uint8_t	 HeaderType;
		uint8_t	 BIST;
	};

	extern const char* DeviceClasses[];

	void		EnumeratePCI( ACPI::MCFG_Header* mcfg );
	const char* GetVendorName( uint16_t vendorID );
	const char* GetDeviceName( uint16_t vendorID, uint16_t deviceID );
	const char* GetSubclassName( uint8_t classCode, uint8_t subclassCode );
	const char* GetProgIF_Name( uint8_t classCode, uint8_t subclassCode, uint8_t progIF );
} // namespace PCI