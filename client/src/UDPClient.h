#pragma once
#include "Utils/Singleton.h"

#include <nlohmann/json.hpp>

#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996) 

#define SERVER_PORT 8888

class UDPClient :
	public Singleton<UDPClient>
{
	friend class Singleton<UDPClient>;

public:
	UDPClient();
	~UDPClient();

	bool Connect(const std::string& ip, const std::string& pseudo);

	const std::string& ReceiveMessage();
	bool SendMsg(const std::string& message);

	bool IsConnected() const { return m_isConnected; }
private:
	void Init();
	void CleanUp();

	SOCKET m_ClientSocket;
	sockaddr_in m_ServerAddr;
	bool m_isInitialized;
	bool m_isConnected;
};

