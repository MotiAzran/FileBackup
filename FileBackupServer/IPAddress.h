#pragma once

#include <cstdint>
#include <string>

class IPAddress
{
public:
	explicit IPAddress(const std::string& ip_address);
	explicit IPAddress(const uint32_t ip_address);
	virtual ~IPAddress() = default;
	IPAddress(const IPAddress&) = default;
	IPAddress& operator=(const IPAddress&) = default;
	IPAddress(IPAddress&&) noexcept = default;
	IPAddress& operator=(IPAddress&&) noexcept = default;

	uint32_t get_ip() const { return _address; }

	static IPAddress localhost() { return IPAddress("127.0.0.1"); }
	static IPAddress any() { return IPAddress("0.0.0.0"); }

private:
	static uint32_t _string_ip_to_dword_ip(const std::string& ip_address);

	uint32_t _address;
};

