#pragma once
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

struct Pong;

class PongDisplay
{
public:

	PongDisplay(sf::Font&);

	void SetScore(unsigned left, unsigned right);
	void SetUsernames(const std::string& left, const std::string& right);
	void Update(nlohmann::json data);
	void Draw(sf::RenderTarget& target) const;

private:

	sf::CircleShape m_Ball;
	sf::RectangleShape m_LeftPaddle;
	sf::RectangleShape m_RightPaddle;
	sf::Text m_ScoreText;
	sf::Text m_leftUsername;
	sf::Text m_rightUsername;
};
