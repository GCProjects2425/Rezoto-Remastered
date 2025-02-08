#pragma once
#include "Utils/Singleton.h"

#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996) 

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

	SOCKET m_ServerSocket;

	bool m_isRunning;
};

