#ifndef _HTTP_REQ_H
#define _HTTP_REQ_H

#ifndef _COMMON_LOG_H
	#include "log.h"
#endif

#ifndef _STR_UTILS_H
	#include "string_utils.h"
#endif

#ifndef _C_LIST_H
	#include "c_list.h"
#endif

class HttpRequest
{
public:
	HttpRequest();
	~HttpRequest() { delete _headers; };

	void SetHeader(const char* header, const char* config);
	void Debug() { _headers->debug(); };

private:
	Group* _headers;
};

#endif