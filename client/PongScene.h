#pragma once
#include "Scene.h"
#include "PongDisplay.h"
#include "Game/Pong.h"
class PongScene :
	public Scene
{
private:

	Pong m_PongGame;
	PongDisplay m_PongDisplay;
	sf::Font m_Font;
	unsigned m_LeftScore, m_RightScore;

public:
	PongScene();

	void Draw(sf::RenderWindow& window) override;
	void Update(float dt)override;
	void Start() override;
	void HandleInput(sf::RenderWindow& window) override;
};

