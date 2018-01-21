#ifndef _HTTP_RES_H
#define _HTTP_RES_H

#ifndef _COMMON_LOG_H
	#include "log.h"
#endif

#ifndef _STR_UTILS_H
	#include "string_utils.h"
#endif

#ifndef _C_LIST_H
	#include "c_list.h"
#endif

#ifndef _COMMON_MSG_H
	#include "message_buf.h"
#endif

class HttpResponse
{
public:
	HttpResponse();
	~HttpResponse() { delete _headers; };

	void SetHeader(const char* header, const char* config);
	void Debug() { _headers->debug(); };

	void SendHeader();
	void Send();

private:
	Group* _headers;
	MessageBuffer _buffer;
};

#endif