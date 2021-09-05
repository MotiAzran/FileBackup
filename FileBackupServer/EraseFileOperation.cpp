#include <cstdio>
#include "ProtocolHandler.h"
#include "EraseFileOperation.h"

EraseFileOperation::EraseFileOperation(const RequestHeader& header, Stream* stream) :
	BaseProtocolOperation(header, stream),
	_filename(ProtocolHandler::read_filename(stream))
{
	if (Operation::EraseFile != header.op)
	{
		throw std::exception("Invalid operation created");
	}
}

void EraseFileOperation::handle_request()
{
	// No user files
	if (is_user_folder_empty())
	{
		ProtocolHandler::write_no_files_error(_stream);
		return;
	}

	// File not exists
	if (!is_file_exists(_filename))
	{
		ProtocolHandler::write_file_not_found_error(_filename, _stream);
		return;
	}

	// Delete file
	std::filesystem::remove(get_file_path(_filename));

	// Send success response
	ResponseHeader header;
	header.version = Common::VERSION;
	header.status = ResponseStatus::EraseFileSuccess;
	ProtocolHandler::write_response_header(header, _stream);
	ProtocolHandler::write_filename(_filename, _stream);
}
