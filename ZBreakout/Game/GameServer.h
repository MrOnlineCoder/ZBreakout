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
#include "../AssetManager.h"

class GameServer {
public:
	GameServer(AssetManager& assets);

	bool start();
	void stop();

	//resets game and server state
	void reset();
private:
	//called in a seperate thread
	//Main server loop
	//Accepts new connections, processes new data and updates game state
	void loop();

	//process single packet from client
	void processPacket(int sender, sf::Packet packet);

	//KICK CLIENTS ASS OUT!
	void kick(int who, std::string msg);

	//sends game state delta (change), called because of tick()
	void sendGameDelta();

	//gives player a weapon
	void givePlayerWeapon(PlayerID id, std::string type);

	//broadcast given packet to all clients
	void broadcast(sf::Packet& packet);

	//Shoots some funky bullets
	void shoot(PlayerID who);

	bool running;
	int connected;

	bool dbgMoveZombies;

	AssetManager& gameAssets;

	Game game;

	sf::Clock clock;

	sf::Thread serverThread;
	sf::UdpSocket udpServer;

	sf::IpAddress clients[Constants::MAX_PLAYERS];
};

#endif
