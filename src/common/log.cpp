#include "log.h"

Log::Log()
{
	//
}

Log::~Log()
{
	//
}

Log* Log::instance()
{
	// returns a singleton of the Log
	static Log instance;
	return &instance;
}

void Log::printMessage(const char* message, ...)
{
	va_list args;
	va_start(args, message);
	// print message in printf format with variable args
	vprintf(message, args);

	va_end(args);
}

void Log::printError(const char* message, ...)
{
	
}

void Log::writeMessage(const char* message, ...)
{
	
}