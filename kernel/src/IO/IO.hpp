#pragma once
#include "../Types/IntDef.hpp"

void outb( uint16_t port, uint8_t byte );
uint8_t inb( uint16_t port );

void io_wait();