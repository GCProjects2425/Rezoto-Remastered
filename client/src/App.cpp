#include "App.h"
#include <cstdlib>
#include "JoinRoomScene.h"

#include <nlohmann/json.hpp>
#include <Game/Enums.h>

using json = nlohmann::json;

App::App()
	: m_Window(sf::VideoMode{sf::Vector2u(sf::Vector2{GameSizeX, GameSizeY})}, "Pong")
	, m_Font("res/fonts/JuliaMono-Regular.ttf")
	, m_Music("res/rick-roll-lofi.ogg")
	, m_PongGame()
	, m_PongDisplay(m_Font)
	, m_LeftScore(0), m_RightScore(0)
	, m_Timer()
	, CurrentScene(nullptr)
{
	m_Window.setFramerateLimit(60);
	//m_Music.setLooping(true);
	//m_Music.setVolume(25.f);
	//m_Music.setPitch(2.f); // LOL
	//m_Music.play();

	m_ClientServer = UDPClient::GetInstance();
}

int App::Run()
{
	if (!m_Window.isOpen())
	{
		return EXIT_FAILURE;
	}

	joinRoomScene = new JoinRoomScene();
	pongScene = new PongScene();

	SetScene(joinRoomScene);

	Timer dtTimer;
	do
	{
		PollEvents();

		float dt = dtTimer.GetElapsedSeconds();
		dtTimer.Restart();


		Update(dt);
		Display();

	} while (m_Window.isOpen());

	return EXIT_SUCCESS;
}

void App::PollEvents()
{
	CurrentScene->HandleInput(m_Window);
}

void App::Update(float dt)
{
	CurrentScene->Update(dt);
	ProcessMessages();
}

void App::Display()
{
	m_Window.clear();
	CurrentScene->Draw(m_Window);
	m_Window.display();
}

void App::ProcessMessages()
{
	if (!m_ClientServer->IsConnected())
		return;

	std::string message = m_ClientServer->ReceiveMessage();
	if (message.empty())
		return;

	try
	{
		json msg = json::parse(message);
		MessageType type = (MessageType)msg["type"];

		switch (type)
		{
		case MessageType::MessageType_Connected:
			joinRoomScene->OnConnectionMessage();
			break;
		case MessageType::MessageType_Update:
			pongScene->OnUpdateMessage(msg["data"]);
			break;
		case MessageType::MessageType_Start:
			joinRoomScene->OnStartMessage();
			break;
		default:
			break;
		}
	}
	catch (const json::exception& e)
	{
		return;
	}
}

void App::SetScene(Scene* newScene)
{
	if (newScene == CurrentScene)
		return;

	CurrentScene = newScene;
	CurrentScene->Start();
}
