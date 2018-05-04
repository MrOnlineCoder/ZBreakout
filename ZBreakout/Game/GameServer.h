/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#ifndef SERVER_H
#define SERVER_H

#include <SFML/Network.hpp>

#include "../Logger.h"
#include "Constants.h"

class GameServer {
public:
	GameServer();
	bool start();
	void stop();
private:
	void loop();
	bool running;

	sf::TcpListener tcpServer;
	sf::Thread serverThread;
};

#endif
