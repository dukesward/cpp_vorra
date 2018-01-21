#include "log.h"

#ifndef _VOR_CORE_H
	#include "core.h"
#endif

#ifndef _C_LIST_H
	#include "c_list.h"
#endif

#ifndef _STR_UTILS_H
	#include "string_utils.h"
#endif

#ifndef _REQ_HANDLER_H
	#include "request_handler.h"
#endif

using namespace std;
// using ioService = boost::asio::io_service;

int main(int argc, char** argv)
{
	// cout << "hello world" << endl;
	LOG_PRINT("Hello AuthServer\n");

	if (!vor.MakeSocket())
    {
        LOG_PRINT("Failed to initialize network\n");
        return 1;
    }
    /*const char* h = "header: test header";
    const char* c = "header: {header_name}";
	HttpRequest* req = new HttpRequest();
	req->SetHeader(h, c);
	req->Debug();*/
    return 0;
}


