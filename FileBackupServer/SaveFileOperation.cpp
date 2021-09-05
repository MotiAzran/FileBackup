#define NOMINMAX

#include <filesystem>
#include "ProtocolHandler.h"
#include "FileStream.h"
#include "SaveFileOperation.h"

SaveFileOperation::SaveFileOperation(const RequestHeader& header, Stream* stream) :
	BaseProtocolOperation(header, stream),
	_filename(ProtocolHandler::read_filename(stream)) 
{
	if (Operation::SaveFile!= header.op)
	{
		throw std::exception("Invalid operation created");
	}
}

void SaveFileOperation::handle_request()
{
	std::filesystem::path file_path = get_file_path(_filename);

	if (std::filesystem::exists(file_path))
	{
		ProtocolHandler::write_general_error(_stream);
		return;
	}

	// Create new file
	FileStream file(file_path);

	// Write received data to file
	receive_payload_to_file(file);
	
	// Send success response
	ResponseHeader header;
	header.version = Common::VERSION;
	header.status = ResponseStatus::SaveFileSuccess;
	ProtocolHandler::write_response_header(header, _stream);
	ProtocolHandler::write_filename(_filename, _stream);
}
