#include "UDPClient.h"

UDPClient::UDPClient()
	: m_ClientSocket(INVALID_SOCKET)
	, m_isInitialized(false)
	, m_isConnected(false)
{
}

void UDPClient::Init()
{
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        return false;
    }
    _isInitialized = true;
}
