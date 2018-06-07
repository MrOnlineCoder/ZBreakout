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
#include "GameProtocol.h"
#include "Game.h"
#include "Level.h"

class GameServer {
public:
	GameServer();

	bool start();
	void stop();
	void reset();
private:
	void loop();
	void processPacket(int sender, sf::Packet packet);
	void kick(int who, std::string msg);
	void sendGameDelta();

	void givePlayerWeapon(PlayerID id, std::string type);

	void broadcast(sf::Packet& packet);

	bool running;
	int connected;

	Game game;

	Level level;

	sf::TcpListener tcpServer;
	sf::Thread serverThread;
	sf::TcpSocket sockets[Constants::MAX_PLAYERS];
};

#endif
