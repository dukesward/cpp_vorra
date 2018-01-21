#ifndef _SOCKET_H
#define _SOCKET_H

#ifndef _COMMON_LOG_H
	#include "log.h"
#endif

#ifndef _SERVERS_H
	#include "servers.h"
#endif

#ifndef _BYTE_BUFFER_H
	#include "byte_buffer.h"
#endif

#ifndef _COMMON_MSG_H
	#include "message_buf.h"
#endif

using String = std::string;
using Tcp = boost::asio::ip::tcp;
using Ip = boost::asio::ip::address;
using SockBase = boost::asio::socket_base;
using ErrCode = boost::system::error_code;
using SysErr = boost::system::system_error;

class Socket : public std::enable_shared_from_this<Socket>
{
public:
	Socket(Tcp::socket&& sock) : _sock(&sock), _remote_addr(sock.remote_endpoint().address()), _readBuffer(), _closed(false), _closing(false)
	{
		LOG_PRINT("Initialize Socket\n");
		_readBuffer.Resize(_READ_BUF_SIZE_);
	};

	~Socket()
	{
		ErrCode error_code;
		_sock->close(error_code);
		_closed = true;
	};

	void AsyncRead()
	{
		_readBuffer.EnsureFreeSpace();

		_sock->async_read_some(
			// a boost asio buffer created from a 2-tuple: buffer(data*, size)
			// the writing point indicates from where the coming buffer is being written
			boost::asio::buffer(_readBuffer.GetWritePointer(), _readBuffer.GetRemainingSpace()),
			// defines how we are going to handle the reading: h(error, size_to_transfer)
			// the binded handler can be called later in a manner such as h(arg1, arg2, ...)
			std::bind(&Socket::_ReadHandler, this->shared_from_this(), std::placeholders::_1, std::placeholders::_2)
		);
	}

	void AsyncWrite(MessageBuffer* buffer)
	{
		_sock->async_write_some(
			// a boost asio buffer created from a 2-tuple: buffer(data*, size)
			// the reading point indicates from where the buffer is read into the socket
			boost::asio::buffer(buffer->GetReadPointer(), buffer->GetActiveSize()),
			// defines how we are going to handle the writing: h(error, size_transfered)
			// the binded handler can be called later in a manner such as h(arg1, arg2, ...)
			std::bind(&Socket::_WriteHandler, this->shared_from_this(), std::placeholders::_1, std::placeholders::_2)
		);
	}

	void CloseSocket()
	{
		ErrCode shut_err;
		_sock->shutdown(SockBase::shutdown_send, shut_err);

		if(shut_err)
			LOG_PRINT("Failed shutting down socket due to %s\n", shut_err.message().c_str());

		_closed = true;
	}

	void Start()
	{
		LOG_PRINT("Start Socket\n");

		std::string ip_address = GetRemoteIpAddress().to_string();
    	LOG_PRINT("Accepted connection from [%s]\n", ip_address.c_str());

		AsyncRead();
	}

	boost::asio::ip::address GetRemoteIpAddress() const
    {
    	LOG_PRINT("Test getting remote ip address...\n");
        return _remote_addr;
    }

    MessageBuffer& GetReadBuffer()
    {
    	return _readBuffer;
    }

protected:
	virtual void ReadHandler() = 0;

	void _ReadHandler(ErrCode error_code, std::size_t trans_b)
	{
		LOG_PRINT("Read handler called with trans bytes: %zu\n", trans_b);

		if(error_code)
		{
			LOG_PRINT("Error reading bytes from socket: %s\n", error_code.message().c_str());
			// in case of reading error, close the socket
			CloseSocket();
		}

		_readBuffer.SetSize(trans_b);
		_readBuffer.WriteCompleted(trans_b);
		// start read handler from the subclass
		ReadHandler();
	}

	void _WriteHandler(ErrCode error_code, std::size_t trans_b)
	{
		LOG_PRINT("Write handler called with trans bytes: %zu\n", trans_b);

		if(error_code)
		{
			LOG_PRINT("Error writing bytes from socket: %s\n", error_code.message().c_str());
			// in case of reading error, close the socket
			CloseSocket();
		}
	}

	Tcp::socket *_sock;
	Ip _remote_addr;
	MessageBuffer _readBuffer;
	bool _closed;
	bool _closing;
};

#endif
