#pragma once

#include "ProtocolStructs.h"
#include "FileStream.h"
#include "IProtocolOperation.h"

class BaseProtocolOperation : public IProtocolOperation
{
public:
	BaseProtocolOperation(const RequestHeader& header, Stream* stream);
	virtual ~BaseProtocolOperation() = default;
	BaseProtocolOperation(const BaseProtocolOperation&) = delete;
	BaseProtocolOperation& operator=(const BaseProtocolOperation&) = delete;
	BaseProtocolOperation(BaseProtocolOperation&&) = delete;
	BaseProtocolOperation& operator=(BaseProtocolOperation&&) = delete;

	virtual void handle_request() = 0;

	static BaseProtocolOperation* create_operation(const Operation op, const RequestHeader& header, Stream* stream);

protected:
	// Return path to file in the user backup folder
	std::filesystem::path get_file_path(const std::string& filename) const;

	// Checks if the user backup folder is empty
	bool is_user_folder_empty() const;

	// Check if file exists in the user backup folder
	bool is_file_exists(const std::string& filename) const;

	// Send file content over the stream 
	void send_file_as_payload(FileStream& file);

	// Receive stream input and write it to file
	void receive_payload_to_file(FileStream& file);

	uint8_t _version;
	uint32_t _user_id;
	Stream* _stream;
};
