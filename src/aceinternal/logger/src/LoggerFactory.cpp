
#include "LoggerFactory.h"
#include "FileLogger.h"

Logger * LoggerFactory::createFileLogger(const string & parameter)
{
	FileLogger * file_logger = new FileLogger();
	bool op_result = file_logger->setFilePath(parameter);
	return file_logger;
}