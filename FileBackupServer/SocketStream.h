#pragma once

#include "Stream.h"
#include "Common.h"

class SocketStream : public Stream
{
public:
	explicit SocketStream(SOCKET sock);
	virtual ~SocketStream();
	SocketStream(const SocketStream&) = delete;
	SocketStream& operator=(const SocketStream&) = delete;
	SocketStream(SocketStream&& other) noexcept;
	SocketStream& operator=(SocketStream&& other) noexcept;

	virtual Buffer read(const uint32_t size) override;
	virtual void write(const Buffer& buf) override;

private:
	SOCKET _sock;
};
