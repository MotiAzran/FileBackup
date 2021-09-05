#include <iostream>
#include <functional>
#include <thread>
#include "AutoWSA.h"
#include "ServerSocket.h"
#include "RequestHandler.h"
#include "BufferUtils.h"

int main()
{
	AutoWSA wsa;
	ServerSocket server(ServerSocket::Host(IPAddress::any(), Common::PORT));

	// Create backup directory if it doesn't exists
	if (!std::filesystem::exists(Common::BACKUP_FOLDER_PATH))
	{
		std::filesystem::create_directory(Common::BACKUP_FOLDER_PATH);
	}

	try
	{
		while (true)
		{
			// Wait for clients
			std::thread([](Stream* s)
				{
					RequestHandler handler(Common::VERSION, s);
					handler.handle();
				},
				new SocketStream(server.accept())).detach();
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Unknown error!" << std::endl;
	}

	return 0;
}