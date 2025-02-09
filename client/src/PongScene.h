#pragma once
#include "Game/Pong.h"
#include "Scene.h"
#include "PongDisplay.h"
#include "UDPClient.h"
class PongScene :
	public Scene
{
private:

	Pong m_PongGame;
	PongDisplay m_PongDisplay;
	sf::Font m_Font;
	std::string m_LeftPlayer, m_RightPlayer;
	unsigned m_LeftScore, m_RightScore;
	bool m_isRunning;
public:
	PongScene();

	void Draw(sf::RenderWindow& window) override;
	void Update(float dt)override;
	void Start() override;
	void HandleInput(sf::RenderWindow& window) override;

	void SetLeftPlayer(const std::string& player) { m_LeftPlayer = player; }
	void SetRightPlayer(const std::string& player) { m_RightPlayer = player; }

	void OnUpdateMessage(nlohmann::json data);
	void OnScoreMessage(nlohmann::json data);
};

