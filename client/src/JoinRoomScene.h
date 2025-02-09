#pragma once
#include "Scene.h"
#include <SFML/Graphics.hpp>
#include <regex>
#include <iostream>
#include "App.h"
#include "UDPClient.h"
enum ConnectionStatus {
    None,
    Connecting,
    WaitingForPlayer
};
class JoinRoomScene : public Scene {
private:
    sf::Font m_Font;
    sf::Text m_UsernameLabel, m_IpLabel;
    sf::Text m_UsernameInput, m_IpInput, m_ErrorMessage;
    sf::Text m_ConnectionStatus;
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

    void Draw(sf::RenderWindow& window) override;
    void Update(float dt) override {}
    void Start() override;
    void HandleInput(sf::RenderWindow& window) override;

    void SwapField();
    void ChangeConnectionStatus(ConnectionStatus status);
};
