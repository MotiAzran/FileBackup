#define NOMINMAX

#include <iostream>
#include <fstream>
#include <filesystem>
#include <exception>
#include "ProtocolHandler.h"
#include "FileStream.h"
#include "BufferUtils.h"
#include "Common.h"
#include "ReturnFileOperation.h"

ReturnFileOperation::ReturnFileOperation(const RequestHeader& header, Stream* stream) :
	BaseProtocolOperation(header, stream),
	_filename(ProtocolHandler::read_filename(_stream))
{
	if (Operation::ReturnFile != header.op)
	{
		throw std::exception("Invalid operation created");
	}
}

void ReturnFileOperation::handle_request()
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

	// Send success response
	ResponseHeader header;
	header.version = Common::VERSION;
	header.status = ResponseStatus::ReturnFileSuccess;
	ProtocolHandler::write_response_header(header, _stream);
	ProtocolHandler::write_filename(_filename, _stream);

	FileStream file(get_file_path(_filename));
	send_file_as_payload(file);	
}
