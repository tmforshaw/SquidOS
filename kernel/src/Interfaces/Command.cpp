#include "CommandManager.hpp"

Command::Command( String name, void ( *function )( TextUI, String ) )
{
	Name = name;
	Function = function;
}

void EchoCommand( TextUI Output, String Params )
{
}

Command CommandArray[] = {
	Command( "echo", &EchoCommand )
};