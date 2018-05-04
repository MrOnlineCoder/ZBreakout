/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#include "GameServer.h"

GameServer::GameServer()
: serverThread(&GameServer::loop, this){
	tcpServer.setBlocking(false);
}

bool GameServer::start() {
	_LOG_.log("GameServer", "Starting GameServer on port "+std::to_string(Constants::GAME_PORT));

	running = true;

	_LOG_.log("GameServer", "Setting up TCP server on port " + std::to_string(Constants::GAME_PORT));

	if (tcpServer.listen(Constants::GAME_PORT) != sf::Socket::Done) {
		_LOG_.log("GameServer", "ERROR: Failed to start TCP server on game port!");
		return false;
	}

	_LOG_.log("GameServer", "Launching server thread...");
	serverThread.launch();

	return true;
}

void GameServer::stop() {
	running = false;

	tcpServer.close();
}

void GameServer::loop() {

	while (running) {


		sf::sleep(sf::milliseconds(1000 / Constants::GAME_TICKRATE));
	}
}
