#pragma once
#include "../Interfaces/TextInterface.hpp"

struct Command
{
public:
	String Name;
	void ( *Function )( TextUI, String );

	Command( String name, void ( *function )( TextUI, String ) );
};

extern Command CommandArray[1];
