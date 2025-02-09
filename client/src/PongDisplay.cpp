#include "PongDisplay.h"
#include "game/Pong.h"

PongDisplay::PongDisplay(sf::Font& font)
	: m_Ball(BallRadius)
	, m_LeftPaddle({ PaddleDistFromBorders, PaddleHeight })
	, m_RightPaddle({ PaddleDistFromBorders, PaddleHeight })
	, m_ScoreText(font, "", 50u)
	, m_leftUsername(font, "", 30u)
	, m_rightUsername(font, "", 30u)

{
	m_Ball.setOrigin({BallRadius, BallRadius});
	m_ScoreText.setFillColor(sf::Color(0xFFFFFF66));
	m_ScoreText.setPosition({GameSizeX / 2.f, GameSizeY / 2.f});

	m_leftUsername.setFillColor(sf::Color(0xFFFFFF66));
	m_leftUsername.setPosition({ GameSizeX / 4.f, GameSizeY / 2.f -50 });

	m_rightUsername.setFillColor(sf::Color(0xFFFFFF66));
	m_rightUsername.setPosition({  GameSizeX -  (GameSizeX / 4.f), GameSizeY / 2.f - 50 });
}

void PongDisplay::SetScore(unsigned left, unsigned right)
{
	m_ScoreText.setString(std::format("{} - {}", left, right));
	m_ScoreText.setOrigin(m_ScoreText.getLocalBounds().getCenter());
}

void PongDisplay::SetUsernames(const std::string& left, const std::string& right)
{
	m_leftUsername.setString(left);
	m_leftUsername.setOrigin(m_ScoreText.getLocalBounds().getCenter());

	m_rightUsername.setString(right);
	m_rightUsername.setOrigin(m_ScoreText.getLocalBounds().getCenter());
}

void PongDisplay::Update(nlohmann::json data)
{
	m_Ball.setPosition({ data["ball"]["x"],data["ball"]["y"] });
	m_LeftPaddle.setPosition({0.f,data["leftPaddle"]});
	m_RightPaddle.setPosition({GameSizeX - PaddleDistFromBorders, data["rightPaddle"] });
}

void PongDisplay::Draw(sf::RenderTarget& target) const
{
	target.draw(m_ScoreText);
	target.draw(m_Ball);
	target.draw(m_LeftPaddle);
	target.draw(m_RightPaddle);
	target.draw(m_leftUsername);
	target.draw(m_rightUsername);
}
