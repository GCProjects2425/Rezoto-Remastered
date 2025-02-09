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

void JoinRoomScene::Draw(sf::RenderWindow& window) 
{
    window.draw(m_UsernameLabel);
    window.draw(m_UsernameInput);
    window.draw(m_IpLabel);
    window.draw(m_IpInput);
    window.draw(m_ErrorMessage);
}

void JoinRoomScene::Start() 
{
    m_UsernameLabel.setFillColor(sf::Color::White);
    m_IpLabel.setFillColor(sf::Color::Color(58, 58, 58));
}

void JoinRoomScene::HandleInput(sf::RenderWindow& window) 
{
    while (const std::optional event = window.pollEvent()) {
        if ((event->is<sf::Event::Closed>()))
            window.close();
        else if (const auto* keyPressed = event->getIf<sf::Event::TextEntered>()) {
            if (m_IsTypingUsername) {
                if (keyPressed->unicode == 8 && !m_Username.empty())
                    m_Username.pop_back();
                else if (keyPressed->unicode >= 32 && keyPressed->unicode <= 126)
                    m_Username += static_cast<char>(keyPressed->unicode);
                m_UsernameInput.setString(m_Username);
            }
            else {
                if (keyPressed->unicode == 8 && !m_Ip.empty())
                    m_Ip.pop_back();
                else if (keyPressed->unicode >= 32 && keyPressed->unicode <= 126)
                    m_Ip += static_cast<char>(keyPressed->unicode);
                m_IpInput.setString(m_Ip);
            }
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->code == sf::Keyboard::Key::Tab) {
                SwapField();
            }
            else if (keyPressed->code == sf::Keyboard::Key::Enter) {
                if (!IsValidUsername(m_Username)) {
                    m_ErrorMessage.setString("Username must be at least 2 characters");
                }
                else if (!IsValidIp(m_Ip)) {
                    m_ErrorMessage.setString("Invalid IP address");
                }
                else {
                    m_ErrorMessage.setString("Connecting...");
                    App::GetInstance()->SetScene(App::GetInstance()->pongScene);
                }
            }
        }
    }
}

void JoinRoomScene::SwapField()
{
    m_IsTypingUsername = !m_IsTypingUsername;
    if (m_IsTypingUsername) {
        m_UsernameLabel.setFillColor(sf::Color::White);
        m_IpLabel.setFillColor(sf::Color::Color(58, 58, 58));
    }
    else {
        m_UsernameLabel.setFillColor(sf::Color::Color(58, 58, 58));
        m_IpLabel.setFillColor(sf::Color::White);
    }
}
