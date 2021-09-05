#include <type_traits>
#include "BufferUtils.h"
#include "ProtocolHandler.h"

template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
Buffer _read_sized_buffer(Stream* stream)
{
	auto size = BufferUtils::buffer_to_pod<T>(stream->read(sizeof(T)));
	return stream->read(size);
}

template <typename T>
void _write_sized_buffer(const Buffer& buf, Stream* stream)
{
	// Write size
	stream->write(BufferUtils::pod_to_buffer(static_cast<T>(buf.size())));

	// Write buffer
	stream->write(buf);
}

Buffer _response_header_to_buffer(const ResponseHeader& header)
{
	auto buffer_size = sizeof(ResponseHeader);
	Buffer buf(buffer_size, 0);

	// Copy header without file name to buffer
	CopyMemory(buf.data(), &header, sizeof(ResponseHeader));

	return buf;
}

RequestHeader ProtocolHandler::read_request_header(Stream* stream)
{
	const uint32_t size_to_read = sizeof(RequestHeader);

	// Read header without file name
	RequestHeader header = { 0 };
	Buffer buf = stream->read(size_to_read);
	CopyMemory(&header, buf.data(), size_to_read);

	return header;
}

std::string ProtocolHandler::read_filename(Stream* stream)
{
	Buffer buf = _read_sized_buffer<uint16_t>(stream);
	return BufferUtils::buffer_to_string(buf);
}

uint32_t ProtocolHandler::read_payload_size(Stream* stream)
{
	return BufferUtils::buffer_to_pod<uint32_t>(stream->read(sizeof(uint32_t)));
}

void ProtocolHandler::write_response_header(const ResponseHeader& header, Stream* stream)
{
	stream->write(_response_header_to_buffer(header));
}

void ProtocolHandler::write_filename(const std::string& filename, Stream* stream)
{
	Buffer buf = BufferUtils::string_to_buffer(filename);
	_write_sized_buffer<uint16_t>(buf, stream);
}

ResponseHeader _create_response_header(const ResponseStatus status)
{
	ResponseHeader header;
	header.version = Common::VERSION;
	header.status = status;

	return header;
}

void ProtocolHandler::write_general_error(Stream* stream)
{
	write_response_header(_create_response_header(ResponseStatus::GeneralError), stream);
}

void ProtocolHandler::write_no_files_error(Stream* stream)
{
	write_response_header(_create_response_header(ResponseStatus::NoClientFiles), stream);
}

void ProtocolHandler::write_file_not_found_error(const std::string& filename, Stream* stream)
{
	write_response_header(_create_response_header(ResponseStatus::FileNotFound), stream);
	write_filename(filename, stream);
}
