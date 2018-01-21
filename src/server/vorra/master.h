#ifndef _VOR_MST_H
#define _VOR_MST_H

#include "kernel.h"
#include "socket.h"

class VorraMaster : public Socket
{
public:
	VorraMaster(Tcp::socket&& sock);
	~VorraMaster();

	void Start();
	char* GetType();

protected:
	void ReadHandler();
	char* type;
};

#endif