#include <random>
#include "TempFileStream.h"

TempFileStream::TempFileStream(const uint32_t path_length) :
	FileStream(_generate_random_filename(path_length)) {}

TempFileStream::~TempFileStream()
{
	try
	{
		delete_file();
	}
	catch (...) {}
}

std::filesystem::path TempFileStream::_generate_random_filename(const uint32_t length)
{
	static const std::string allowed_chars("0123456789BCDFGHJKLMNPQRSTVWXZbcdfghjklmnpqrstvwxz");

	std::mt19937 random_gen(std::random_device{}());
	std::uniform_int_distribution<std::string::size_type> random_dist(0, allowed_chars.size() - 1);

	// Generate random name
	std::string name;
	for (uint32_t i = 0; i < length; ++i)
	{
		name.push_back(allowed_chars[random_dist(random_gen)]);
	}

	return name;
}
