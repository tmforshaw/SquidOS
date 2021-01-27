#pragma once
#include "../Interfaces/TextInterface.hpp"
#include "../Types/String.hpp"

class CommandManager
{
private:
	TextUI CommandLineUI;

public:
	CommandManager();
	CommandManager( Point pos, uint16_t width, uint16_t height );

	void SendCommand();
	void Display();
};

extern CommandManager GlobalCommand;
extern TextUI* CommandLineUI;