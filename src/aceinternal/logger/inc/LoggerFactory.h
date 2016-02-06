
#ifndef LOGGER_FACTORY_HPP
#define LOGGER_FACTORY_HPP

#include "Logger.h"

class LOGGER_EXOPRT LoggerFactory
{
public:
	static Logger * createFileLogger(const string & parameter);
};

#endif