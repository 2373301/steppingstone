
#include "TerminalFactory.h"
#include "ManageSingleTerminal.h"
#include "ManageGateTerminal.h"
#include "GateTerminal.h"

ManageTerminal * TerminalFactory::createManageTerminal(int type_value)
{
	if (0 == type_value)
	{
		return ManageSingleTerminal::instance();
	}
	else
	{
		return new ManageGateTerminal();
	}
}

Terminal * TerminalFactory::createTerminal(int type_value)
{
	if (0 == type_value)
	{
		return new SingleTerminal();
	}
	else
	{
		return new GateTerminal();
	}
}