#pragma once
#include "../Interfaces/TextInterface.hpp"
#include "../Types/String.hpp"
#include "Command.hpp"

class CommandManager
{
private:
	TextUI CommandLineUI;

public:
	CommandManager( PointU pos, uint16_t width, uint16_t height );

	TextUI& GetTextUI();

	void SendCommand( String command, String params );
	void Display();
};

extern CommandManager GlobalCommand;