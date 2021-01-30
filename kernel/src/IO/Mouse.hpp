#pragma once
#include "../Math/Math.hpp"

#define PS2LeftButton	0b00000001 // Left button bit
#define PS2MiddleButton 0b00000010 // Middle button bit
#define PS2RightButton	0b00000100 // Right button bit

#define PS2XSign	 0b00010000 // X sign bit
#define PS2YSign	 0b00100000 // Y sign bit
#define PS2XOverflow 0b01000000 // X overflow bit
#define PS2YOverflow 0b10000000 // Y overflow bit

void HandlePS2Mouse( uint8_t data );
void ProcessMousePacket();
void InitPS2Mouse();

extern Point MousePosition;