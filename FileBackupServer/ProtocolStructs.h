#pragma once

#include "Common.h"

// Available server operations
enum class Operation : uint8_t
{
	SaveFile = 100,
	ReturnFile = 200,
	EraseFile,
	GetFileList
};

// Possible server response statuses
enum class ResponseStatus : uint16_t
{
	// Success statusses
	ReturnFileSuccess = 210,
	GetFileListSuccess = 211,
	SaveFileSuccess = 212,
	EraseFileSuccess = 212,

	// Error statuses
	FileNotFound = 1001,
	NoClientFiles = 1002,
	GeneralError = 1003
};

// Set alignment to 1
#pragma pack(push, 1)
typedef struct _request_header
{
	uint32_t user_id;
	uint8_t version;
	Operation op;
} RequestHeader;

typedef struct _response_header
{
	uint8_t version;
	ResponseStatus status;
} ResponseHeader;
#pragma pack(pop)

typedef struct _request
{
	RequestHeader header;
	std::string filename;
	Buffer payload;
} Request;

typedef struct _response
{
	ResponseHeader header;
	std::string filename;
	Buffer payload;
} Response;
