#pragma once
#include "Scene.h"
#include <SFML/Graphics.hpp>
#include <regex>
#include <iostream>
#include "App.h"

class JoinRoomScene : public Scene {
private:
    sf::Font m_Font;
    sf::Text m_UsernameLabel, m_IpLabel;
    sf::Text m_UsernameInput, m_IpInput, m_ErrorMessage;
    std::string m_Username, m_Ip;
    bool m_IsTypingUsername = true;

    bool IsValidIp(const std::string& ip) {
        std::regex ipPattern("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
        return std::regex_match(ip, ipPattern);
    }

    bool IsValidUsername(const std::string& username) {
        return username.length() >= 2;
    }

public:

    JoinRoomScene();

    void Draw(sf::RenderWindow& window) override {
        window.draw(m_UsernameLabel);
        window.draw(m_UsernameInput);
        window.draw(m_IpLabel);
        window.draw(m_IpInput);
        window.draw(m_ErrorMessage);
    }

    void Update(float dt) override {}

    void Start() override {}

    void HandleInput(sf::RenderWindow& window) override {
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
                    m_IsTypingUsername = !m_IsTypingUsername;
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
};
