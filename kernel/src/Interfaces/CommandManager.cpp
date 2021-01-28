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

void CommandManager::SendCommand( String command )
{
	// Process the commandText here
	String Params = command.GetAfterWord( 0 );
	String CommandWord = command.GetWord( 0 );

	GlobalRenderer->Print( CommandWord );
	GlobalRenderer->Endl();

	for ( Command com : CommandArray )
	{
		if ( CommandWord == com.Name )
		{
			com.Function( CommandLineUI, Params );
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