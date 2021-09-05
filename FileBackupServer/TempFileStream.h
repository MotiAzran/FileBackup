#pragma once

#include <cstdlib>
#include "FileStream.h"

class TempFileStream : public FileStream
{
public:
	explicit TempFileStream(const uint32_t path_length=32);
	virtual ~TempFileStream();
	TempFileStream(const TempFileStream&) = delete;
	TempFileStream& operator=(const TempFileStream&) = delete;
	TempFileStream(TempFileStream&& other) = default;
	TempFileStream& operator=(TempFileStream&& other) = default;

private:
	static std::filesystem::path _generate_random_filename(const uint32_t length);
};
