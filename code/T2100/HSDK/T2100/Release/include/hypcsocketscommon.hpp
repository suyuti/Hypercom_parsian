//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2004
//=============================================================================

#ifndef _HYPCSOCKETSCOMMON_HPP_
#define _HYPCSOCKETSCOMMON_HPP_

#include "Sockets.h"
#include "HypCConfig.hpp"

class HypCSocketsCommon
{
public:
	virtual ~HypCSocketsCommon() {}

	// create socket
	virtual SOCKET  socket(int, int, int) = 0;
	
	// bind socket
	virtual int 	bind(SOCKET, struct sockaddr*, int) = 0;

	// listen socket
	virtual int	listen(SOCKET s, int backlog) = 0;


	//accept socket 
	virtual SOCKET accept(SOCKET s, struct sockaddr *addr, int* addrlen) = 0;
	
	// connect socket
	virtual int 	connect(SOCKET, struct sockaddr*, int) = 0;

	// socket operations
	virtual int 	ioctlsocket(SOCKET, long, UDWORD*) = 0;
	
	// send data to socket
	virtual int 	send(SOCKET, const char*, int, int) = 0;
	
	// get received data from socket
	virtual int 	recv(SOCKET, char *, int, int) = 0;
	
	// get received data from socket with timeout
	virtual int 	recv_to(SOCKET, char *, int, int, int) = 0;
	
	// close connection and destroy socket
	virtual int 	closesocket(SOCKET) = 0;

	// send for UDP
	virtual int 	sendto(SOCKET, const char*, int, int, struct sockaddr*, int) = 0;
	
	// receive for UDP
	virtual int 	recvfrom(SOCKET, char*, int, int, struct sockaddr*, int*) = 0;

	// get last error code
	virtual int		lasterror() = 0;

	// set configuration options
	virtual void	SetConfig(const HypCConfig &cfg) = 0;

	// get configuration options
	virtual void	GetConfig(HypCConfig &param) = 0;

protected:
	HypCConfig		m_LocalConfig;

};

#endif	// _HYPCSOCKETSCOMMON_HPP_
