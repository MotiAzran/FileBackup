#pragma once

#include "SocketStream.h"
#include "IPAddress.h"
#include "Common.h"

class ServerSocket
{
public:
	using Port = uint16_t;
	using Host = std::pair<IPAddress, Port>;

public:
	ServerSocket(const Host& host);
	virtual ~ServerSocket();
	ServerSocket(const ServerSocket&) = delete;
	ServerSocket& operator=(const ServerSocket&) = delete;
	ServerSocket(ServerSocket&& other) noexcept;
	ServerSocket& operator=(ServerSocket&& other) noexcept;

	SocketStream accept();

private:
	static SOCKET _create_listen_socket(const Host& host);

	SOCKET _listen_socket;
};
