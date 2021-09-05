#pragma once

#include <string>
#include "BaseProtocolOperation.h"

class EraseFileOperation : public BaseProtocolOperation
{
public:
	explicit EraseFileOperation(const RequestHeader& header, Stream* stream);
	virtual ~EraseFileOperation() = default;
	EraseFileOperation(const EraseFileOperation&) = delete;
	EraseFileOperation& operator=(const EraseFileOperation&) = delete;
	EraseFileOperation(EraseFileOperation&&) = delete;
	EraseFileOperation& operator=(EraseFileOperation&&) = delete;

	virtual void handle_request() override;

private:
	std::string _filename;
};

