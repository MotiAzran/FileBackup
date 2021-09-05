#pragma once

#include <memory>
#include <string>
#include "Stream.h"
#include "ProtocolStructs.h"
#include "Common.h"

namespace ProtocolHandler
{
	RequestHeader read_request_header(Stream* stream);
	std::string read_filename(Stream* stream);
	uint32_t read_payload_size(Stream* stream);

	void write_response_header(const ResponseHeader& header, Stream* stream);
	void write_filename(const std::string& filename, Stream* stream);

	void write_general_error(Stream* stream);
	void write_no_files_error(Stream* stream);
	void write_file_not_found_error(const std::string& filename, Stream* stream);
};
