#include "JoinRoomScene.h"
#include <nlohmann/json.hpp>

#include "Game/Enums.h"

using json = nlohmann::json;

JoinRoomScene::JoinRoomScene()
    : m_UsernameLabel(m_Font, "", 50u),
    m_IpLabel(m_Font, "", 50u),
    m_UsernameInput(m_Font, "", 50u),
    m_IpInput(m_Font, "", 50u),
    m_ErrorMessage(m_Font, "", 50u),
    m_Font("./res/fonts/JuliaMono-Regular.ttf") ,
    m_ConnectionStatus(m_Font,"",50u)
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

    m_ConnectionStatus.setFont(m_Font);
    m_ConnectionStatus.setPosition(sf::Vector2f(50, 350));
    m_ConnectionStatus.setCharacterSize(24);
    m_ConnectionStatus.setFillColor(sf::Color::Green);
}

void JoinRoomScene::Draw(sf::RenderWindow& window) 
{
    window.draw(m_UsernameLabel);
    window.draw(m_UsernameInput);
    window.draw(m_IpLabel);
    window.draw(m_IpInput);
    window.draw(m_ErrorMessage);
    window.draw(m_ConnectionStatus);
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
                    //m_ErrorMessage.setString("Connecting...");
                    //App::GetInstance()->SetScene(App::GetInstance()->pongScene);
                    if (UDPClient::GetInstance()->Connect(m_Ip, m_Username))
                    {
						ChangeConnectionStatus(Connecting);
						json msg = {
							{"type", MessageType::MessageType_Connect},
							{"data", {
								{"username", m_Username }
							}}
						};
						UDPClient::GetInstance()->SendMsg(msg.dump());
					}
                    else
                    {
                        m_ErrorMessage.setString("Error while connecting");
                        ChangeConnectionStatus(None);
                    }
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

void JoinRoomScene::ChangeConnectionStatus(ConnectionStatus status)
{
    switch (status)
    {
    case None:
        m_ConnectionStatus.setString("");
        break;
    case Connecting:
        m_ConnectionStatus.setFillColor(sf::Color::Color(152, 251, 152));
        m_ConnectionStatus.setString("Connecting to the server ...");
        break;
    case WaitingForPlayer:
        m_ConnectionStatus.setFillColor(sf::Color::Color(32, 178, 170));
        m_ConnectionStatus.setString("Waiting for player to join ...");
        break;
    default:
        break;
    }
}

void JoinRoomScene::OnConnectionMessage()
{
    	ChangeConnectionStatus(WaitingForPlayer);
}
