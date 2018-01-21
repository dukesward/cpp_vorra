#ifndef _SOCKET_MGR_H
#define _SOCKET_MGR_H

#ifndef _COMMON_LOG_H
	#include "log.h"
#endif

#ifndef _SERVERS_H
	#include "servers.h"
#endif

#include "socket_pool.h"

using String = std::string;
using ioService = boost::asio::io_service;
using Tcp = boost::asio::ip::tcp;
using Ip = boost::asio::ip::address;
using SockBase = boost::asio::socket_base;
using ErrCode = boost::system::error_code;
using SysErr = boost::system::system_error;

template<class SocketType>
class SocketMgr
{
public:
	// type define for async accept callback function
	typedef void(*AcceptCallback)(Tcp::socket&& sock);

	bool StartNetwork()
	{
        if (!Connect())
        {
            LOG_PRINT("StartNetwork failed to connect with socket acceptor\n");
            return false;
        }

        LOG_PRINT("StartNetwork binded successfully\n");
        return true;
	}

	void OnSocketOpen(Tcp::socket&& sock)
	{
		try
		{
			// we make a shared ptr of type SocketType to make sure it does not go out of the scope
			std::shared_ptr<SocketType> _socket = std::make_shared<SocketType>(std::move(sock));
			// allocate the socket into the pool
			// AllocateSocket(std::move(*_socket));
			// start socket for async reading
			_socket->Start();
			// close the socket if just to test the behavior of the acceptor
			// Close();
		}
		catch(SysErr const& system_error)
		{
			LOG_PRINT("Failed to retrieve client remote address: %s\n", system_error.what());
		}
	}

	void Accept()
	{
		// create a socket for moving
		Tcp::socket* sock;
		// we will deal with the thread later
		int thread = 0;
		// to initialize the socket we need to tie a tuple to it & an unint
		// here we want the socket & unint to be tied to a pair
		// the std::tie will bind the tuple for us
		std::tie(sock, thread) = BindSocket();
		// async accept from a connection into a socket with handler
		_acceptor.async_accept(
			// the socket will be used to recv data from the connection
			*sock,
			// define a lambda function for callback when connection is coming
			[this, sock](ErrCode error_code)
			{
				// if no error encountered when accepting, proceeding the socket reading
				if(!error_code)
				{
					try
					{
						// set the socket as non-blocking so we can read it with async reader
						sock->non_blocking(true);
						// start proceeding the socket operation in provided callback
						// notice that sock will not be used outside the lambda fn, so we can move it
						// since sock is a lvalue, we must apply std::move to convert it to an rvalue
						OnSocketOpen(std::move(*sock));
					}
					catch(SysErr const& system_error)
					{
						LOG_PRINT("Failed to initialize network from client connection: %s", system_error.what());
					}
				}

				if(!_closed)
					this->Accept();
			}
		);
	}

	bool Connect()
	{
		// create a new boost err code
		ErrCode err_code;
		// prepare the acceptor with auto detected protocol
		_acceptor.open(_end_point.protocol(), err_code);
		// log out the open err if err code is given
		if(err_code)
		{
			LOG_PRINT("Error openning async acceptor: %s\n", err_code.message().c_str());
			return false;
		}
		// bind the acceptor on the end point for connection
		_acceptor.bind(_end_point, err_code);
		// log out the bind err if err code is given
		if(err_code)
		{
			LOG_PRINT("Error binding async acceptor: %s\n", err_code.message().c_str());
			// close the socket if called when in use, as this should never happen
			Close();
			return false;
		}
		// listenning on the port with specified max connections [128]
		_acceptor.listen(SockBase::max_connections, err_code);
		// log out the listen err if err code is given
		if(err_code)
		{
			LOG_PRINT("Error listening async acceptor: %s\n", err_code.message().c_str());
			return false;
		}

		LOG_PRINT("Successfully binded async acceptor on %s:%d\n", _end_point.address().to_string().c_str(), _end_point.port());

		return true;
	};

	void Close()
	{
		LOG_PRINT("Closing socket...\n");

		ErrCode error_code;
		// close the acceptor
		_acceptor.close(error_code);
	};

protected:
	SocketMgr(boost::asio::io_service& ios, String const& host, uint16 port)
		: _ios(&ios), _acceptor(ios), _end_point(Ip::from_string(host), port), _sock(ios), _closed(false)
    {
    	//
    	if(!Connect())
    		LOG_PRINT("Failed establishing socket connection\n");

    	Accept();
    	_ios->run();
    }

    ioService* _ios;
    Tcp::acceptor _acceptor;
	Tcp::endpoint _end_point;
	Tcp::socket _sock;
	bool _closed;

private:
	std::pair<Tcp::socket*, uint32> BindSocket()
	{
		return std::make_pair(&_sock, 0);
	};
};

#endif