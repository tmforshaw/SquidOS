#include "CommandManager.hpp"

#include "../Rendering/BasicRenderer.hpp"
#include "../Types/String.hpp"

CommandManager GlobalCommand = CommandManager( { 0, 0 }, 0, 0 );

CommandManager::CommandManager( Point pos, uint16_t width, uint16_t height )
{
	CommandLineUI = TextUI( pos, width, height );

	CommandLineUI.isCommandLine = true;
}

TextUI& CommandManager::GetTextUI()
{
	return this->CommandLineUI;
}

void CommandManager::SendCommand( String command, String params )
{
	for ( Command com : CommandArray )
	{
		if ( command == com.Name )
		{
			com.Function( CommandLineUI, params );
			GlobalRenderer->Print( "Command Called" );
			GlobalRenderer->Endl();
			return;
		}
	}
}

void CommandManager::Display()
{
	CommandLineUI.Display();
}