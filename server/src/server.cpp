#include "Utils/Console.h"
#include <mutex>
#include <functional>
#include "UDPServer.h"
#include "GameServerGUI.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	using namespace Console;
	Out << WindowTitle::SetTitle("Pong Server Console")
		<< TextColors::BrightFgYellow << ">>> The server is starting!\n";

	GameServerGUI gui(hInstance);
	std::thread guiThread(std::bind(&GameServerGUI::Run, &gui));
	SetThreadDescription((HANDLE)guiThread.native_handle(), L"GuiThread");

	std::thread networkThread(std::bind(&UDPServer::Run, UDPServer::GetInstance()));
	SetThreadDescription((HANDLE)networkThread.native_handle(), L"NetworkThread");

	networkThread.join();
	guiThread.join();

	Out << TextColors::FgCyan << ">>> The server has been shut down.\n";

	return 0;
}
