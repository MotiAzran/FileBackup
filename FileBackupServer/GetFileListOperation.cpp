#define NOMINMAX

#include "ProtocolHandler.h"
#include "BufferUtils.h"
#include "TempFileStream.h"
#include "GetFileListOperation.h"

GetFileListOperation::GetFileListOperation(const RequestHeader& header, Stream* stream) :
	BaseProtocolOperation(header, stream) 
{
	if (Operation::GetFileList != header.op)
	{
		throw std::exception("Invalid operation created");
	}
}

void GetFileListOperation::handle_request()
{
	// No user files
	if (is_user_folder_empty())
	{
		ProtocolHandler::write_no_files_error(_stream);
		return;
	}

	// Write files list to temporary file
	TempFileStream tmp_file;
	std::filesystem::path user_folder_path = Common::BACKUP_FOLDER_PATH / std::to_string(_user_id);
	for (const auto& file : std::filesystem::directory_iterator(user_folder_path))
	{
		tmp_file.write(file.path().filename().string());
		tmp_file.write("\n");
	}
	tmp_file.flush();

	// Send success response
	ResponseHeader header;
	header.version = Common::VERSION;
	header.status = ResponseStatus::GetFileListSuccess;
	ProtocolHandler::write_response_header(header, _stream);
	ProtocolHandler::write_filename(tmp_file.get_path().string(), _stream);
	send_file_as_payload(tmp_file);
}
