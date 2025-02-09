#include "UDPClient.h"

UDPClient::UDPClient()
	: m_ClientSocket(INVALID_SOCKET)
	, m_isInitialized(false)
	, m_isConnected(false)
{
    Init();
}

UDPClient::~UDPClient()
{
	CleanUp();
}

bool UDPClient::Connect(const std::string& ip, const std::string& pseudo)
{
	if (!m_isInitialized)
		return false;
	if (m_isConnected)
		return false;

	m_ClientSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_ClientSocket == INVALID_SOCKET) {
		std::string message = "Error on socket creation: " + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, message.c_str(), "UDP ERROR", MB_OK | MB_ICONERROR);
		WSACleanup();
		return false;
	}

	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);
	server.sin_addr.s_addr = inet_addr(ip.c_str());

	if (connect(m_ClientSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		std::string message = "Error on connect: " + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, message.c_str(), "UDP ERROR", MB_OK | MB_ICONERROR);
		closesocket(m_ClientSocket);
		WSACleanup();
		return false;
	}

	m_isConnected = true;
	return true;
}

const std::string& UDPClient::ReceiveMessage()
{
	char buffer[1025];
	int bytesReceived = recv(m_ClientSocket, buffer, 1024, 0);
	if (bytesReceived == SOCKET_ERROR) {
		int error = WSAGetLastError();
		if (error == WSAECONNRESET || error == WSAECONNABORTED)
		{
			m_isConnected = false;
			CleanUp();
		}
		if (error != WSAEWOULDBLOCK) {
			std::string message = "Error on recv: " + std::to_string(error);
			MessageBoxA(NULL, message.c_str(), "UDP ERROR", MB_OK | MB_ICONERROR);
		}

		return "";
	}
	buffer[bytesReceived] = '\0';

	return std::string(buffer, bytesReceived+1);
}

void UDPClient::Init()
{
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		std::string message = "WSAStartup failed: " + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, message.c_str(), "UDP ERROR", MB_OK | MB_ICONERROR);
		return;
	}
}

void UDPClient::CleanUp()
{
	if (m_ClientSocket != INVALID_SOCKET) 
	{
		closesocket(m_ClientSocket);
		m_ClientSocket = INVALID_SOCKET;
	}

	if (m_isInitialized) 
	{
		WSACleanup();
		m_isInitialized = false;
	}
}
