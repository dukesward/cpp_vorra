#include "request_handler.h"

std::unordered_map<std::string, std::string> RequestHandler::InitHeaderConfig()
{
	std::unordered_map<std::string, std::string> _config;

	_config[_HEADER_HTTP_ROUTE]      = "{method} /{route} HTTP/{http_v}";
	_config[_HEADER_HTTP_HOST]       = "Host: {host_addr}:{host_port}";
	_config[_HEADER_HTTP_CONN]       = "Connection: {conn_type}";
	_config[_HEADER_HTTP_CACHE_CTL]  = "Cache-Control: max-age={cache_max_age}";
	_config[_HEADER_HTTP_USR_AGENT]  = "User-Agent: {user_agent}";
	_config[_HEADER_HTTP_ACCEPT]     = "Accept: {accept}";
	_config[_HEADER_HTTP_ENCODING]   = "Accept-Encoding: {accept_encoding}";
	_config[_HEADER_HTTP_LANGUAGE]   = "Accept-Language: {accept_language}";

	return _config;
}

std::unordered_map<std::string, std::string> const HeaderConfig = RequestHandler::InitHeaderConfig();

RequestHandler::RequestHandler(Tcp::socket&& sock) : Socket(std::move(sock))
{
	//
}

void RequestHandler::ReadHandler()
{
	LOG_PRINT("Start ReadHandler from RequestHandler\n");
	// use this to check if the request is in correct format
	// _readBuffer.PrintStorage();
	// 
	HttpRequest* req = new HttpRequest();
	HttpResponse* res = new HttpResponse();
	// store accepted request headers into the request
	_Handle(req);
	Send(res);
}

void RequestHandler::PrepareResponse()
{

}

void RequestHandler::Send(HttpResponse* _res)
{
	if (_closed)
        return;

    MessageBuffer buffer;

    const char* test = "HTTP/1.1 200 OK\r\nContent-type:text/html\r\n\r\n<h1>Hello Vorra</h1>\r\n";
    buffer.WriteBuffer<char>(test, strlen(test));

    AsyncWrite(&buffer);
}

void RequestHandler::_Handle(HttpRequest* _req)
{
	// the split should be a const to avoid throwing a warning
	// since c++03 a string literal is treated like a char const*
	char const* split = "\r\n";
	// HttpRequest* _req;
	// Group* g = new Group();
	_readBuffer.ReadWithSplit<uint8>(
		// this is equivalent to a read-by-line mechanism [\r => 13, \n => 10]
		split,
		[_req](std::string&& token)
		{
			// LOG_PRINT("Test fetch http request header line = %s\n", token.c_str());
			// look for tokens in each header accepted, and store in the list
			for(auto i=HeaderConfig.begin(); i!=HeaderConfig.end(); ++i)
			{
				if(StringUtils::IndexOf(token.c_str(), i->first.c_str()) >= 0)
				{
					_req->SetHeader(token.c_str(), i->second.c_str());
					// StringUtils::Search(token.c_str(), i->second.c_str(), g);
				}
			}
		}
	);
	// _req->Debug();
}