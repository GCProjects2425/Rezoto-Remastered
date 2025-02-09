#include "UDPServer.h"

#include "Utils/Console.h"
#include "nlohmann/json.hpp"
#include <Game/Enums.h>

using namespace Console;
using json = nlohmann::json;

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
		Out << TextColors::BgRed << ">>> WSAStartup failed: " << WSAGetLastError() << "\n";

	Launch();
}

void UDPServer::Launch()
{
	sockaddr_in server;
	int addrLength = sizeof(server);

	if ((m_ServerSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
		Out << TextColors::BgRed << ">>> Could not create socket: " << WSAGetLastError() << "\n";
		return;
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	if (bind(m_ServerSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
		Out << TextColors::BgRed << ">>> Bind failed with error code: " << WSAGetLastError() << "\n";
		return;
	}

	u_long mode = 1;
	ioctlsocket(m_ServerSocket, FIONBIO, &mode);

	m_isRunning = true;
	Out << TextColors::FgGreen << ">>> UDP Server is running on "<< inet_ntoa(server.sin_addr) << ":" << ntohs(server.sin_port) << "!\n";
}

void UDPServer::HandleMessages()
{
	sockaddr_in client;
	int addrLength = sizeof(client);
	char buffer[1025];
	int bytesReceived = recvfrom(m_ServerSocket, buffer, 1024, 0, (struct sockaddr*)&client, &addrLength);
	if (bytesReceived == SOCKET_ERROR) {
		int error = WSAGetLastError();
		if (error != WSAEWOULDBLOCK) {
			Out << TextColors::BgRed << ">>> recvfrom failed with error code: " + std::to_string(error) << "\n";
		}
		return;
	}
	buffer[bytesReceived] = '\0';

	std::string clientId = inet_ntoa(client.sin_addr);

	if (m_Players.find(clientId) == m_Players.end())
	{
		Player player(clientId, "Player" + std::to_string(m_Players.size()), ntohs(client.sin_port));
		m_Players[clientId] = player;
	}

	json recvbufJson;
	try 
	{
		recvbufJson = json::parse(std::string(buffer, bytesReceived+1));
	}
	catch (const json::parse_error& e) 
	{
		Out << TextColors::BgRed << ">>> JSON parse error: " << e.what() << "\n";
		return;
	}
	ProcessMessage(clientId, recvbufJson);
}

void UDPServer::ProcessMessage(const std::string& clientID, json content)
{
	Out << TextColors::BgRed << ">>> Message received from " << clientID << ": " << content.dump() << "\n";

	switch ((MessageType)content["type"])
	{
	case MessageType::MessageType_Connect:
	{
		std::string username = content["data"]["username"];
		OnPlayerConnect(clientID, username);
		Out << TextColors::FgGreen << ">>> " << clientID << " connected with username: " << username << "\n";
		break;
	}
	default:
		break;
	}
}

void UDPServer::OnPlayerConnect(const std::string& clientID, const std::string& username)
{
	m_Players[clientID].username = username;
	m_Players[clientID].connectionStart = time(0);
	json resp = {
		{"type", MessageType::MessageType_Connected}
	};
}

void UDPServer::SendMsg(const std::string& clientID, const std::string& message)
{
	if (m_Players.find(clientID) == m_Players.end())
	{
		Out << TextColors::BgRed << ">>> Player not found: " << clientID << "\n";
		return;
	}
	sockaddr_in client;
	client.sin_family = AF_INET;
	client.sin_port = htons(m_Players[clientID].port);
	client.sin_addr.s_addr = inet_addr(clientID.c_str());
	std::string msg = message;
	if (sendto(m_ServerSocket, msg.c_str(), msg.size(), 0, (struct sockaddr*)&client, sizeof(client)) == SOCKET_ERROR)
	{
		Out << TextColors::BgRed << ">>> sendto failed with error code: " << WSAGetLastError() << "\n";
	}
}
