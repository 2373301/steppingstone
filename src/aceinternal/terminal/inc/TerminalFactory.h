
#ifndef TERMINAL_FACTORY_HPP
#define TERMINAL_FACTORY_HPP

#include "Terminal.h"
#include "ManageTerminal.h"
#include "Terminal_def.h"

class TERMINAL_EXOPRT TerminalFactory
{
public:
	static ManageTerminal * createManageTerminal(int type_value);

	static Terminal * createTerminal(int type_value);
protected:
private:
};
#endif