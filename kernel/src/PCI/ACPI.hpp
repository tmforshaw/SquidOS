#pragma once
#include "../Types/IntDef.hpp"

namespace ACPI
{
	struct RSDP2
	{
		unsigned char Signature[8];
		uint8_t CheckSum;
		uint8_t OEM_ID[6];
		uint8_t Revision;
		uint32_t RSDT_Address;
		uint32_t Length;
		uint64_t XSDT_Address; // Extended System Descriptor Table
		uint8_t ExtendedCheckSum;
		uint8_t Reserve[3];
	} __attribute__( ( packed ) );

	struct SDT_Header
	{
		unsigned char Signature[4];
		uint32_t Length;
		uint8_t Revision;
		uint8_t CheckSum;
		uint8_t OEM_ID[6];
		uint8_t OEM_TableID[8];
		uint32_t OEM_Revision;
		uint32_t CreatorID;
		uint32_t CreatorRevision;
	} __attribute__( ( packed ) );

	struct MCFG_Header // Contains all the info about the PCI Bus
	{
		SDT_Header Header;
		uint64_t ReservedBytes;
	} __attribute__( ( packed ) );

	struct DeviceConfig
	{
		uint64_t BaseAddress;
		uint16_t PCI_SegGroup;
		uint8_t StartBus;
		uint8_t EndBus;
		uint32_t Reserve;
	} __attribute__( ( packed ) );

	void*
	FindTable( SDT_Header* sdtHeader, char* signature );
}; // namespace ACPI