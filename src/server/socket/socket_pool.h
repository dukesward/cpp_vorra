#ifndef _SOCKET_POOL_H
#define _SOCKET_POOL_H

#ifndef _COMMON_LOG_H
	#include "log.h"
#endif

#ifndef _SERVERS_H
	#include "servers.h"
#endif

#include "response.h"

// this allows at most 50 responses to be temp stored
#define MAX_POOL_SIZE 50

class SocketPool
{
public:
	// copy constructor is not allowed as only one master pool should exit
	SocketPool(SocketPool const&) = delete;
	// the singleton makes sure the static is always destroyed
	static SocketPool& Instance()
    {
        static SocketPool instance;
        return instance;
    }

    void allocate(HttpResponse&& res)
    {
    	LOG_PRINT("Allocating socket\n");
    	// adding new socket to the socket list
    	if(_res)
    	{
    		if(!_next)
    			_next = new SocketPool();
    		
    		_next->allocate(res);
    	}
    	else
			std::swap(_res, res);
    }

private:
	// can only be called by the class itself
	SocketPool() : _next(nullptr), _node(nullptr) {};
	// use _node to store a socket, and use _next to link the next
	HttpResponse* _res;
	SocketPool* _next;

	static SocketPool instance;
};

#define _pool SocketPool::Instance()

#endif