#pragma once
#include "Utils/Singleton.h"

#include <nlohmann/json.hpp>

#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996) 

class UDPClient :
	public Singleton<UDPClient>
{
	friend class Singleton<UDPClient>;

public:
	UDPClient();

private:
	void Init();

	SOCKET m_ClientSocket;
	bool m_isInitialized;
	bool m_isConnected;
};

