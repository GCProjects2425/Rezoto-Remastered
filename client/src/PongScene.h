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
	unsigned m_LeftScore, m_RightScore;
	bool m_isRunning;
public:
	PongScene();

	void Draw(sf::RenderWindow& window) override;
	void Update(float dt)override;
	void Start() override;
	void HandleInput(sf::RenderWindow& window) override;

	void OnUpdateMessage(nlohmann::json data);
	void OnScoreMessage(nlohmann::json data);
};

