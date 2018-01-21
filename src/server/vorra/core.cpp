#include "core.h"

Vorra& Vorra::Instance()
{
	// create a new boost io_service shared ptr
	// we want it to be a shared ptr so that it won't run out of scope
	// when an io_services runs, it blocks util either all processes are finished or it's stopped
	std::shared_ptr<boost::asio::io_service> ioService = std::make_shared<boost::asio::io_service>();
	std::string host = "192.168.2.111";
	int32 port = 8087;

	static Vorra* instance = new Vorra(*ioService, host, port);
	return *instance;
}

void Vorra::StartService(Tcp::socket&& sock)
{
	Instance().OnSocketOpen(std::forward<Tcp::socket>(sock));
}

Vorra::Vorra(boost::asio::io_service& ios, String const& host, uint16 port)
	: SocketMgr(ios, host, port)
{

}

Vorra::~Vorra()
{

}

bool Vorra::MakeSocket()
{
	return true;
}