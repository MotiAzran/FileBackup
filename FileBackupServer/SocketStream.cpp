#include "SocketStream.h"

SocketStream::SocketStream(SOCKET sock) :
	_sock(sock)
{
	if (INVALID_SOCKET == sock)
	{
		throw std::exception("Got invalid socket value");
	}
}

SocketStream::~SocketStream()
{
	try
	{
		if (INVALID_SOCKET != _sock)
		{
			closesocket(_sock);
			shutdown(_sock, SD_BOTH);
		}
	} catch(...) {}
}

SocketStream::SocketStream(SocketStream&& other) noexcept :
	_sock(other._sock)
{
	other._sock = INVALID_SOCKET;
}

SocketStream& SocketStream::operator=(SocketStream&& other) noexcept
{
	_sock = other._sock;
	other._sock = INVALID_SOCKET;

	return *this;
}

Buffer SocketStream::read(const uint32_t size)
{
	if (0 == size)
	{
		return Buffer();
	}

	Buffer buf(size, 0);

	if (SOCKET_ERROR == ::recv(_sock, reinterpret_cast<char*>(buf.data()), size, MSG_WAITALL))
	{
		throw std::exception("Receive data failed");
	}

	return buf;
}

void SocketStream::write(const Buffer& buf)
{
	if (buf.empty())
	{
		throw std::exception("Can't send empty buffer");
	}

	if (SOCKET_ERROR == ::send(_sock, reinterpret_cast<const char*>(buf.data()), buf.size(), 0))
	{
		throw std::exception("Send data failed");
	}
}
