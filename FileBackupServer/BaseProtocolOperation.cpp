#define NOMINMAX

#include <exception>
#include "ProtocolHandler.h"
#include "BufferUtils.h"
#include "SaveFileOperation.h"
#include "ReturnFileOperation.h"
#include "EraseFileOperation.h"
#include "GetFileListOperation.h"
#include "BaseProtocolOperation.h"

BaseProtocolOperation::BaseProtocolOperation(const RequestHeader& header, Stream* stream) :
	_version(header.version),
	_user_id(header.user_id),
	_stream(stream)
{
	if (nullptr == stream)
	{
		throw std::exception("Invalid stream");
	}
}

std::filesystem::path BaseProtocolOperation::get_file_path(const std::string& filename) const
{
	return Common::BACKUP_FOLDER_PATH / std::to_string(_user_id) / filename;
}

bool BaseProtocolOperation::is_user_folder_empty() const
{
	std::filesystem::path user_folder_path = Common::BACKUP_FOLDER_PATH / std::to_string(_user_id);
	
	return std::filesystem::is_empty(user_folder_path);
}

bool BaseProtocolOperation::is_file_exists(const std::string& filename) const
{
	return std::filesystem::exists(get_file_path(filename));
}

void BaseProtocolOperation::send_file_as_payload(FileStream& file)
{
	uint32_t size_to_send = file.get_file_size();

	// Send file size
	_stream->write(BufferUtils::pod_to_buffer(size_to_send));

	// Send file in chunks
	while (size_to_send > 0)
	{
		uint32_t read_size = std::min(size_to_send, Common::CHUNK_SIZE);
		auto data = file.read(read_size);
		_stream->write(data);
		size_to_send -= read_size;
	}
}

void BaseProtocolOperation::receive_payload_to_file(FileStream& file)
{
	// Read file size
	const auto file_size = ProtocolHandler::read_payload_size(_stream);

	// write contents to file in chunks
	uint32_t bytes_written = 0;
	while (bytes_written < file_size)
	{
		auto read_size = std::min(Common::CHUNK_SIZE, file_size - bytes_written);
		auto buf = _stream->read(read_size);
		file.write(buf);
		bytes_written += buf.size();
	}
}

BaseProtocolOperation* BaseProtocolOperation::create_operation(const Operation op, const RequestHeader& header, Stream* stream)
{
	switch (op)
	{
	case Operation::SaveFile:
		return new SaveFileOperation(header, stream);
	case Operation::ReturnFile:
		return new ReturnFileOperation(header, stream);
	case Operation::EraseFile:
		return new EraseFileOperation(header, stream);
	case Operation::GetFileList:
		return new GetFileListOperation(header, stream);
	default:
		throw std::exception("Invalid operation given");
	}
}
