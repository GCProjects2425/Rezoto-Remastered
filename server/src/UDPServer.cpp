#include "UDPServer.h"

#include "Utils/Console.h"

using namespace Console;

UDPServer::UDPServer()
	: m_ServerSocket(INVALID_SOCKET)
	, m_isRunning(false)
{
	Init();
}

void UDPServer::Run()
{
	while (m_isRunning)
	{
		
	}
}

void UDPServer::Init()
{
	WSADATA wsa;

	int result = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (result != 0)
		Out << TextColors::BgRed << ">>> WSAStartup failed: " << std::to_string(result) << "\n";

	Launch();
}

void UDPServer::Launch()
{
	sockaddr_in server;
	int addrLength = sizeof(server);

	if ((m_ServerSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
		Out << TextColors::BgRed << ">>> Could not create socket: " + WSAGetLastError() << "\n";
		return;
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	if (bind(m_ServerSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
		Out << TextColors::BgRed << ">>> Bind failed with error code: " + WSAGetLastError() << "\n";
		return;
	}

	u_long mode = 1;
	ioctlsocket(m_ServerSocket, FIONBIO, &mode);

	m_isRunning = true;
	Out << TextColors::FgGreen << ">>> UDP Server is running on "<< inet_ntoa(server.sin_addr) << ":" << ntohs(server.sin_port) << "!\n";
}
