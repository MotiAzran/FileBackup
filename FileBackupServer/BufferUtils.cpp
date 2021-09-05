#include "BufferUtils.h"

Buffer BufferUtils::string_to_buffer(const std::string& str)
{
	return Buffer(str.cbegin(), str.cend());
}

std::string BufferUtils::buffer_to_string(const Buffer& buf)
{
	return std::string(buf.cbegin(), buf.cend());
}
