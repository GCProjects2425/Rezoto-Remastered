#pragma once
#include <string>
#include <ctime>

struct Player
{
	std::string ip;
	std::string username;
	time_t connectionStart;
	time_t lastPing;
	int port;

	Player() : ip(""), username(""), port(0), connectionStart(0), lastPing(0) {}
	Player(const std::string& ip, const std::string& username, int port) : ip(ip), username(username), port(port)
	{
		connectionStart = time(0);
		lastPing = connectionStart;
	}
};

class Party
{
};

