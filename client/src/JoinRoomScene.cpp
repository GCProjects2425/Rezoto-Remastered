#include "JoinRoomScene.h"

JoinRoomScene::JoinRoomScene()
    : m_UsernameLabel(m_Font, "", 50u),
    m_IpLabel(m_Font, "", 50u),
    m_UsernameInput(m_Font, "", 50u),
    m_IpInput(m_Font, "", 50u),
    m_ErrorMessage(m_Font, "", 50u),
    m_Font("./res/fonts/JuliaMono-Regular.ttf") 
{

    m_UsernameLabel.setFont(m_Font);
    m_UsernameLabel.setString("Username:");
    m_UsernameLabel.setPosition(sf::Vector2f(50, 50));
    m_UsernameLabel.setCharacterSize(24);

    m_IpLabel.setFont(m_Font);
    m_IpLabel.setString("Server IP:");
    m_IpLabel.setPosition(sf::Vector2f(50, 150));
    m_IpLabel.setCharacterSize(24);

    m_UsernameInput.setFont(m_Font);
    m_UsernameInput.setPosition(sf::Vector2f(200, 50));
    m_UsernameInput.setCharacterSize(24);

    m_IpInput.setFont(m_Font);
    m_IpInput.setPosition(sf::Vector2f(200, 150));
    m_IpInput.setCharacterSize(24);

    m_ErrorMessage.setFont(m_Font);
    m_ErrorMessage.setPosition(sf::Vector2f(50, 250));
    m_ErrorMessage.setCharacterSize(24);
    m_ErrorMessage.setFillColor(sf::Color::Red);
}
