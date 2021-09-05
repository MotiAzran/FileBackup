#pragma once

#include "BaseProtocolOperation.h"

class SaveFileOperation : public BaseProtocolOperation
{
public:
	explicit SaveFileOperation(const RequestHeader& header, Stream* stream);
	virtual ~SaveFileOperation() = default;
	SaveFileOperation(const SaveFileOperation&) = delete;
	SaveFileOperation& operator=(const SaveFileOperation&) = delete;
	SaveFileOperation(SaveFileOperation&&) = delete;
	SaveFileOperation& operator=(SaveFileOperation&&) = delete;

	virtual void handle_request() override;

private:
	std::string _filename;
};

