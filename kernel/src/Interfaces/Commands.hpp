#pragma once
#include "../Interfaces/TextInterface.hpp"
#include "../Types/String.hpp"

class CommandManager
{
private:
public:
	CommandManager();

	void AddChar( char chr );
	void Backspace();
	void SendCommand();
};

extern CommandManager GlobalCommand;
extern TextUI* CommandLineUI;