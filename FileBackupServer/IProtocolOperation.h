#pragma once

#include <memory>
#include "Stream.h"
#include "ProtocolStructs.h"

class IProtocolOperation
{
public:
	virtual void handle_request() = 0;
};
