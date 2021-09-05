#pragma once

#include "Common.h"

// Start the WSA at ctor, and close it at the dtor
class AutoWSA
{
public:
	AutoWSA();
	virtual ~AutoWSA();
	AutoWSA(const AutoWSA&) = delete;
	AutoWSA& operator=(const AutoWSA&) = delete;
	AutoWSA(AutoWSA&&) noexcept = delete;
	AutoWSA& operator=(AutoWSA&&) noexcept = delete;

private:
	static bool _is_initialized;
};

