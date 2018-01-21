#include "response.h"

HttpResponse::HttpResponse() : _headers(), _buffer()
{
	_headers = new Group();
}

void HttpResponse::SetHeader(const char* header, const char* config)
{
	StringUtils::Search(header, config, _headers);
}

void SendHeader()
{
	
}

void Send()
{
	// _buffer.WriteBuffer<char>(test, strlen(test));
}