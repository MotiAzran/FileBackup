#include <exception>
#include "AutoWSA.h"

bool AutoWSA::_is_initialized = false;

AutoWSA::AutoWSA()
{
	if (_is_initialized)
	{
		return;
	}

	WSADATA wsa_data = { 0 };
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa_data))
	{
		throw std::exception("WSAStartup failed");
	}

	_is_initialized = true;
}

AutoWSA::~AutoWSA()
{
	try
	{
		WSACleanup();
		_is_initialized = false;
	} catch (...) {}
}
