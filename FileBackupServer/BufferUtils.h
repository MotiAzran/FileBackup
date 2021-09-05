#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <type_traits>
#include "Common.h"

using Buffer = std::vector<uint8_t>;

namespace BufferUtils
{
	Buffer string_to_buffer(const std::string& str);

	template <typename T, typename = std::enable_if_t<std::is_pod_v<T>>>
	Buffer pod_to_buffer(T val)
	{
		Buffer buf(sizeof(val), 0);
		CopyMemory(buf.data(), &val, sizeof(val));

		return buf;
	}

	std::string buffer_to_string(const Buffer& buf);

	template <typename T, typename = std::enable_if_t<std::is_pod_v<T>>>
	T buffer_to_pod(const Buffer& buf)
	{
		T val;
		CopyMemory(&val, buf.data(), sizeof(T));

		return val;
	}
}