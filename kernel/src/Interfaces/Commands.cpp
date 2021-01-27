#include "Commands.hpp"

#include "../Rendering/BasicRenderer.hpp"
#include "../Types/String.hpp"

CommandManager GlobalCommand;

CommandManager::CommandManager()
{
	CommandLineUI = TextUI( { 0, 0 }, 0, 0 );
	CommandLineUI.isCommandLine = true;
	// SelectedTextUI = &CommandLineUI; // Select this text box
}

CommandManager::CommandManager( Point pos, uint16_t width, uint16_t height )
{
	CommandLineUI = TextUI( pos, width, height );

	CommandLineUI.isCommandLine = true;
	SelectedTextUI = &CommandLineUI; // Select this text box
}

void CommandManager::SendCommand()
{
	// Process the commandText here
}

void CommandManager::Display()
{
	CommandLineUI.Display();
}