#ifndef _VOR_CORE_H
#define _VOR_CORE_H

#ifndef _COMMON_LOG_H
	#include "log.h"
#endif

#ifndef _COMMON_MSG_H
	#include "message_buf.h"
#endif

#ifndef _SOCKET_MGR_H
	#include "socket_mgr.h"
#endif

#ifndef _REQ_HANDLER_H
	#include "request_handler.h"
#endif

#include "master.h"
#include "kernel.h"

using String = std::string;
using Tcp = boost::asio::ip::tcp;
using Ip = boost::asio::ip::address;
using SockBase = boost::asio::socket_base;
using ErrCode = boost::system::error_code;
using SysErr = boost::system::system_error;
using ioService = boost::asio::io_service;

class Vorra : public SocketMgr<RequestHandler>
{
public:
	typedef SocketMgr<RequestHandler> _SocketMgr;

	static Vorra& Instance();
	// make these delete so it raises compile err if they are called
	Vorra(Vorra const&)          = delete;
	void operator=(Vorra const&) = delete;

	bool MakeSocket();

protected:
	Vorra(boost::asio::io_service& ios, String const& host, uint16 port);
	~Vorra();

	static void StartService(Tcp::socket&& sock);
};

#define vor Vorra::Instance()

#endif