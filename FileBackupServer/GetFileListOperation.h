#pragma once

#include "BaseProtocolOperation.h"

class GetFileListOperation : public BaseProtocolOperation
{
public:
	explicit GetFileListOperation(const RequestHeader& header, Stream* stream);
	virtual ~GetFileListOperation() = default;
	GetFileListOperation(const GetFileListOperation&) = delete;
	GetFileListOperation& operator=(const GetFileListOperation&) = delete;
	GetFileListOperation(GetFileListOperation&&) = delete;
	GetFileListOperation& operator=(GetFileListOperation&&) = delete;

	virtual void handle_request() override;
};

