#include "PongScene.h"
#include <nlohmann/json.hpp>
#include <Game/Enums.h>

void PongScene::Draw(sf::RenderWindow& window)
{
	m_PongDisplay.Draw(window);
}

void PongScene::Update(float dt)
{
	//if (!m_isRunning)
	//{
	//	m_isRunning = true;
	//	m_PongGame.Reset();
	//}

	////m_PongGame.Update(dt);
	//m_PongDisplay.Update(m_PongGame);

	//switch (m_PongGame.GetGameState())
	//{
	//case GameState::LeftWins:
	//{
	//	++m_LeftScore;
	//	break;
	//}
	//case GameState::RightWins:
	//{
	//	++m_RightScore;
	//	break;
	//}
	//default: return;
	//}

	//m_PongGame.Reset();
	//m_PongDisplay.SetScore(m_LeftScore, m_RightScore);
}

void PongScene::Start()
{
}

static PaddlesBehaviour operator|=(PaddlesBehaviour& lhs, PaddlesBehaviour rhs)
{
	using UType = std::underlying_type_t<PaddlesBehaviour>;
	return lhs = static_cast<PaddlesBehaviour>(static_cast<UType>(lhs) | static_cast<UType>(rhs));
}

static PaddlesBehaviour operator&=(PaddlesBehaviour& lhs, PaddlesBehaviour rhs)
{
	using UType = std::underlying_type_t<PaddlesBehaviour>;
	return lhs = static_cast<PaddlesBehaviour>(static_cast<UType>(lhs) & static_cast<UType>(rhs));
}

static PaddlesBehaviour operator~(PaddlesBehaviour rhs)
{
	using UType = std::underlying_type_t<PaddlesBehaviour>;
	return static_cast<PaddlesBehaviour>(~static_cast<UType>(rhs));
}

void PongScene::HandleInput(sf::RenderWindow& window)
{
	while (const std::optional event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
			window.close();

		if (auto* keyEvent = event->getIf<sf::Event::KeyPressed>())
		{
			using enum sf::Keyboard::Key;
			using enum PaddlesBehaviour;
			switch (keyEvent->code)
			{
			case W:
			case Z:
			case Up:
			{
				nlohmann::json msg = {
					{"type", MessageType::MessageType_StartMovingPaddle},
					{"data", { {"Paddle", "UP"}}}
				};
				UDPClient::GetInstance()->SendMsg(msg.dump());
				break;
			}
			case Down:
			case S:
			{
				nlohmann::json msg = {
					{"type", MessageType::MessageType_StartMovingPaddle},
					{"data", { {"Paddle", "DOWN"}}}
				};
				UDPClient::GetInstance()->SendMsg(msg.dump());
				break;
			}
			}
		}

		if (auto* keyEvent = event->getIf<sf::Event::KeyReleased>())
		{
			using enum sf::Keyboard::Key;
			using enum PaddlesBehaviour;
			switch (keyEvent->code)
			{
			case W:
			case Z:
			case Up:
			{
				nlohmann::json msg = {
					{"type", MessageType::MessageType_StopMovingPaddle},
					{"data", { {"Paddle", "UP"}}}
				};
				UDPClient::GetInstance()->SendMsg(msg.dump());
				break;
			}
			case Down:
			case S:
			{
				nlohmann::json msg = {
					{"type", MessageType::MessageType_StopMovingPaddle},
					{"data", { {"Paddle", "DOWN"}}}
				};
				UDPClient::GetInstance()->SendMsg(msg.dump());
				break;
			}
			}
		}
	}

}

void PongScene::OnUpdateMessage(nlohmann::json data)
{
	if (!m_isRunning)
	{
		m_isRunning = true;
		m_PongGame.Reset();
	}

	//m_PongGame.Update(dt);
	m_PongDisplay.Update(data);

	switch (m_PongGame.GetGameState())
	{
	case GameState::LeftWins:
	{
		++m_LeftScore;
		break;
	}
	case GameState::RightWins:
	{
		++m_RightScore;
		break;
	}
	default: return;
	}

	m_PongGame.Reset();
	m_PongDisplay.SetScore(m_LeftScore, m_RightScore);
}


PongScene::PongScene()
	:
	m_Font("./res/fonts/JuliaMono-Regular.ttf"),
	m_PongGame(),
	m_PongDisplay(m_Font)
	, m_LeftScore(0), m_RightScore(0)
	, m_isRunning(false)
{
}
