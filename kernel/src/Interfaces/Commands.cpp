#include "Commands.hpp"

#include "../Rendering/BasicRenderer.hpp"
#include "../Types/String.hpp"

CommandManager GlobalCommand = CommandManager();
TextUI* CommandLineUI;

String commandText;

CommandManager::CommandManager()
{
	commandText = String( "" );
}

void CommandManager::AddChar( char chr )
{
	commandText += chr;
}

void CommandManager::Backspace()
{
	commandText.Split( 0, commandText.Length() - 2 ); // Remove the last char
}

void CommandManager::SendCommand()
{
	// Process the commandText here

	GlobalRenderer->Print( commandText );

	commandText = "";
}