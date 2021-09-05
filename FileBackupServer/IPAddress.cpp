#include <ws2tcpip.h>
#include "Common.h"
#include "IPAddress.h"

IPAddress::IPAddress(const std::string& ip_address) :
	_address(_string_ip_to_dword_ip(ip_address)) {}

IPAddress::IPAddress(const uint32_t ip_address) :
	_address(ip_address) {}

uint32_t IPAddress::_string_ip_to_dword_ip(const std::string& ip_address)
{
	uint32_t address = 0;

	// Convert string ip address to number
	if (!InetPtonA(AF_INET, ip_address.c_str(), &address))
	{
		throw std::exception("Error serializing ip address");
	}

	return address;
}
