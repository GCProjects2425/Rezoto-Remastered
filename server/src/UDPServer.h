#pragma once
#include "Utils/Singleton.h"
#include <unordered_map>
#include <nlohmann/json.hpp>

#include <Game/Pong.h>

#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996) 

#include "Party.h"

#define PORT 8888

class UDPServer :
	public Singleton<UDPServer>
{
	friend class Singleton<UDPServer>;

public:
	UDPServer();

	void Run();
private:
	void Init();
	void Launch();

	void StartGame();

	void HandleMessages();
	void ProcessMessage(const std::string& clientID, nlohmann::json content);

	void OnPlayerConnect(const std::string& clientID, const std::string& username);

	void SendGameUpdate();
	void SendMsg(const std::string& clientID, const std::string& message);

	SOCKET m_ServerSocket;
	std::unordered_map<std::string, Party> m_Parties;
	std::unordered_map<std::string, Player> m_Players;
	std::string m_LeftPlayer, m_RightPlayer;
	Pong m_PongGame;
	bool m_isRunning;
	bool m_isGameRunning;
};

