#include "ServerSocket.h"

ServerSocket::ServerSocket(const Host& host) :
	_listen_socket(_create_listen_socket(host)) {}

ServerSocket::~ServerSocket()
{
	try
	{
		if (INVALID_SOCKET != _listen_socket)
		{
			closesocket(_listen_socket);
		}
	} catch(...) {}
}

ServerSocket::ServerSocket(ServerSocket&& other) noexcept :
	_listen_socket(other._listen_socket)
{
	other._listen_socket = INVALID_SOCKET;
}

ServerSocket& ServerSocket::operator=(ServerSocket&& other) noexcept
{
	_listen_socket = other._listen_socket;
	other._listen_socket = INVALID_SOCKET;

	return *this;
}

SocketStream ServerSocket::accept()
{
	SOCKET sock = ::accept(_listen_socket, nullptr, nullptr);
	if (INVALID_SOCKET == sock)
	{
		throw std::exception("Error while getting new connection");
	}

	return SocketStream(sock);
}

SOCKET ServerSocket::_create_listen_socket(const Host& host)
{
	// Create socket
	SOCKET sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sock)
	{
		throw std::exception("Failed creating server socket");
	}

	// Set host
	struct sockaddr_in service = { 0 };
	service.sin_family = AF_INET;
	service.sin_port = htons(std::get<Port>(host));
	service.sin_addr.s_addr = std::get<IPAddress>(host).get_ip();

	// Bind port
	if (SOCKET_ERROR == ::bind(sock, reinterpret_cast<sockaddr*>(&service), sizeof(service)))
	{
		throw std::exception("Failed binding server socket");
	}

	// Set listen
	if (SOCKET_ERROR == ::listen(sock, SOMAXCONN))
	{
		throw std::exception("listen failed");
	}

	return sock;
}
