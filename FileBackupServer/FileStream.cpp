#define NOMINMAX

#include <filesystem>
#include "FileStream.h"

FileStream::FileStream(const std::filesystem::path& path) :
	_path(path),
	_ifile(path, std::ios::app | std::ios::binary),
	_ofile(path, std::ios::app | std::ios::binary)
{
	// Validate file opened
	if (!_ifile.is_open() || !_ofile.is_open())
	{
		throw std::exception("Failed creting file");
	}
}

FileStream::FileStream(const std::string& path) :
	FileStream(std::filesystem::path(path)) {}

Buffer FileStream::read(const uint32_t size)
{
	// Validate file still opened
	if (!_ifile.is_open() || !_ofile.is_open())
	{
		throw std::exception("File closed");
	}

	if (0 == size)
	{
		return Buffer();
	}

	// Set size to read, the minimum from the given size to the file size
	auto file_size = get_file_size();
	uint32_t size_to_read = std::min(size, file_size);
	Buffer buf(size_to_read, 0);

	// Read from the file
	_ifile.read(reinterpret_cast<char*>(buf.data()), size_to_read);

	return buf;
}

void FileStream::write(const Buffer& buf)
{
	// Validate the file still opened
	if (!_ifile.is_open() || !_ofile.is_open())
	{
		throw std::exception("File closed");
	}

	// Write buffer to the file
	_ofile.write(reinterpret_cast<const char*>(buf.data()), buf.size());
}

void FileStream::write(const std::string& str)
{
	write(BufferUtils::string_to_buffer(str));
}

void FileStream::flush()
{
	_ofile.flush();
}

uint32_t FileStream::get_file_size() const
{
	return static_cast<uint32_t>(std::filesystem::file_size(_path));
}

void FileStream::delete_file()
{
	// Close file before deletion
	_ifile.close();
	_ofile.close();

	// Delete file
	std::filesystem::remove(_path);
}
