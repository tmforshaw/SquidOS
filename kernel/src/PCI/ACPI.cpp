#include "ACPI.hpp"

namespace ACPI
{
	void* FindTable( SDT_Header* sdtHeader, char* signature )
	{
		uint32_t entriesLength = ( sdtHeader->Length - sizeof( ACPI::SDT_Header ) ) / 8;

		for ( uint16_t i = 0; i < entriesLength; i++ )
		{
			ACPI::SDT_Header* newSDT_Header = (ACPI::SDT_Header*)( *(uint64_t*)( (uint64_t)sdtHeader + sizeof( ACPI::SDT_Header ) + ( i * 8 ) ) );

			for ( uint16_t j = 0; j < 4; j++ )
			{
				if ( newSDT_Header->Signature[j] != signature[j] ) break;

				if ( j == 3 ) return newSDT_Header;
			}
		}

		return nullptr;
	}

} // namespace ACPI