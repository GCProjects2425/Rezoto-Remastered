#pragma once
#include "Game/Pong.h"
#include "PongDisplay.h"
#include "Utils/Timer.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "PongScene.h"
#include "Utils/Singleton.h"

class JoinRoomScene;
class App : public Singleton<App>
{
public:

	App();
	int Run();

	void SetScene (Scene* newScene);
	PongScene* pongScene;
	JoinRoomScene* joinRoomScene;


private:

	void PollEvents();
	void Update(float dt);
	void Display();

	Scene* CurrentScene;


private:

	sf::RenderWindow m_Window;
	sf::Font m_Font;
	sf::Music m_Music;

	Pong m_PongGame;
	PongDisplay m_PongDisplay;
	unsigned m_LeftScore, m_RightScore;

	Timer m_Timer;
};
