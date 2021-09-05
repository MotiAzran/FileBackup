#pragma once

#include <fstream>
#include <string>
#include <filesystem>
#include "Stream.h"

class FileStream : public Stream
{
public:
	explicit FileStream(const std::filesystem::path& path);
	explicit FileStream(const std::string& path);
	virtual ~FileStream() = default;
	FileStream(const FileStream&) = delete;
	FileStream& operator=(const FileStream&) = delete;
	FileStream(FileStream&& other) = default;
	FileStream& operator=(FileStream&& other) = default;

	uint32_t get_file_size() const;
	std::filesystem::path get_path() const { return _path; }

	virtual Buffer read(const uint32_t size);
	virtual void write(const Buffer& buf);
	virtual void write(const std::string& str);

	void flush();

	void delete_file();

protected:
	std::filesystem::path _path;
	std::ifstream _ifile;
	std::ofstream _ofile;
};

