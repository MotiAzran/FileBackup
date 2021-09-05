#pragma once

#include "Stream.h"

class RequestHandler
{
public:
	explicit RequestHandler(const uint8_t version, Stream* stream);
	virtual ~RequestHandler();
	RequestHandler(const RequestHandler&) = delete;
	RequestHandler& operator=(const RequestHandler&) = delete;
	RequestHandler(RequestHandler&&) = delete;
	RequestHandler& operator=(RequestHandler&&) = delete;

	void handle();

private:
	void _safe_handle();

	uint8_t _version;
	Stream* _stream;
};
