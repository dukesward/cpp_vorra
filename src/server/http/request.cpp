#include "request.h"

HttpRequest::HttpRequest() : _headers()
{
	_headers = new Group();
}

void HttpRequest::SetHeader(const char* header, const char* config)
{
	StringUtils::Search(header, config, _headers);
}