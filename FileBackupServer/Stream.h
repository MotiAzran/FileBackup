#pragma once

#include "BufferUtils.h"

class Stream
{
public:
	virtual Buffer read(const uint32_t size) = 0;
	virtual void write(const Buffer& buf) = 0;
};
