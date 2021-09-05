#pragma once

#include <cstdint>
#include <filesystem>
#include <vector>
#include <string>

#include <WinSock2.h>
#include <Windows.h>

#pragma comment(lib, "ws2_32.lib")

namespace Common
{
	static const std::filesystem::path BACKUP_FOLDER_PATH = "C:\\BackupSrv\\";
	constexpr uint16_t PORT = 7777;
	static const uint8_t VERSION = 1;
	static const uint32_t CHUNK_SIZE = 64 * 1024;
}
