#pragma once

#include <string>
#include "BaseProtocolOperation.h"
#include "Common.h"

class ReturnFileOperation : public BaseProtocolOperation
{
public:
	explicit ReturnFileOperation(const RequestHeader& header, Stream* stream);
	virtual ~ReturnFileOperation() = default;
	ReturnFileOperation(const ReturnFileOperation&) = delete;
	ReturnFileOperation& operator=(const ReturnFileOperation&) = delete;
	ReturnFileOperation(ReturnFileOperation&&) = delete;
	ReturnFileOperation& operator=(ReturnFileOperation&&) = delete;

	virtual void handle_request() override;

private:
	std::string _filename;
};
