#include "log.h"

#ifndef _VOR_CORE_H
    #include "core.h"
#endif

#ifndef _C_LIST_H
    #include "c_list.h"
#endif

#ifndef _NUM_UTILS_H
    #include "numeric.h"
#endif

#ifndef _STR_UTILS_H
    #include "string_utils.h"
#endif

using namespace std;
// using ioService = boost::asio::io_service;

int main(int argc, char** argv)
{
	// cout << "hello world" << endl;
	LOG_PRINT("Start Testing\n");

	/*if (!vor.MakeSocket())
    {
        LOG_PRINT("Failed to initialize network\n");
        return 1;
    }*/

    const char* src = "GET / HTTP/1.1";
    //const char* hint = "{method} /{(/)route} HTTP/{http_v}";
    //C_List list;
    //StringUtils::Search(src, hint, &list);
    int i = StringUtils::IndexOf(src, "HTTP");
    LOG_PRINT("Found [HTTP] in [%s] with index [%d]\n", src, i);

    return 0;
}