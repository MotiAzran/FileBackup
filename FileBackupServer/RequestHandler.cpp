#include <iostream>
#include <filesystem>
#include "ProtocolHandler.h"
#include "BaseProtocolOperation.h"
#include "RequestHandler.h"

RequestHandler::RequestHandler(const uint8_t version, Stream* stream) :
	_version(version),
	_stream(stream) {}

RequestHandler::~RequestHandler()
{
	try
	{
		delete _stream;
	}
	catch (...) {}
}	

void RequestHandler::_safe_handle()
{
	BaseProtocolOperation* operation = nullptr;
	try
	{
		// Read request header and validate version
		auto request_header = ProtocolHandler::read_request_header(_stream);

		// Create user backup directory if needed
		std::filesystem::path user_folder_path = Common::BACKUP_FOLDER_PATH / std::to_string(request_header.user_id);
		if (!std::filesystem::exists(user_folder_path))
		{
			std::filesystem::create_directory(user_folder_path);
		}

		// Handle request
		operation = BaseProtocolOperation::create_operation(request_header.op, request_header, _stream);
 		operation->handle_request();
		delete operation;
	}
	catch (...)
	{
		// Free operation if needed
		if (nullptr != operation)
		{
			delete operation;
		}

		// Error occured send general error to client
		ProtocolHandler::write_general_error(_stream);
	}
}

void RequestHandler::handle()
{
	// Another try..catch block for communication problem
	try
	{
		_safe_handle();
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Unknown error!" << std::endl;
	}
}
