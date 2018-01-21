#ifndef _COMMON_EXCEP_H
#define _COMMON_EXCEP_H

#ifndef _GLOB_DEFINE_H
	#include "define.h"
#endif

class VorraError : public std::runtime_error
{
public:
	explicit VorraException(const char* message) 
		: _msg(message)
	{

	}

	explicit VorraException(char&& message)
		: _msg(&message)
	{

	}

	virtual ~VorraException() throw() {};

	virtual const char* what() const throw() { return _msg; };

protected:
	const char* _msg;
};

#endif