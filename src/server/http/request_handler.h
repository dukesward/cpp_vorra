#ifndef _REQ_HANDLER_H
#define _REQ_HANDLER_H

#ifndef _COMMON_LOG_H
	#include "log.h"
#endif

#ifndef _STR_UTILS_H
	#include "string_utils.h"
#endif

#ifndef _C_LIST_H
	#include "c_list.h"
#endif

#ifndef _SOCKET_H
	#include "socket.h"
#endif

#include "request.h"
#include "response.h"

class RequestHandler : public Socket
{
public:
	static std::unordered_map<std::string, std::string> InitHeaderConfig();
	RequestHandler(Tcp::socket&& sock);

protected:
	void ReadHandler();
	void PrepareResponse();
	void Send(HttpResponse* _res);

private:
	void _Handle(HttpRequest* _req);
};

#endif