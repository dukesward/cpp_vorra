#ifndef _COMMON_LOG_H
#define _COMMON_LOG_H

#ifndef _GLOB_DEFINE_H
	#include "define.h"
#endif

#define _LOG_ROOT_ "logs"

class COMMON_API Log
{
	private:
		Log();
		~Log();

	public:
		static Log* instance();

		void printMessage(const char* message, ...);
		void printError(const char* message, ...);
		void writeMessage(const char* message, ...);
};

#define _LOG_ Log::instance()

#define LOG_PRINT(message, ...)                       \
		_LOG_->printMessage(message, ##__VA_ARGS__);  \

#define LOG_ERR(message, ...)                         \
		_LOG_->printError(message, ##__VA_ARGS__);    \

#endif