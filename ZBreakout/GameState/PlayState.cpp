/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/
//Project includes
#include "PlayState.h"

PlayState::PlayState(StateManager* mgr)
	: GameState(mgr) {

	status.setFont(manager->getAssets().getFont("main"));
	status.setPosition(25, manager->getWindowSize().y - status.getCharacterSize() - 25);
	status.setString("Connecting to server...");
	status.setFillColor(sf::Color::White);

	socket.setBlocking(false);

	playerView.setSize(sf::Vector2f(mgr->getWindowSize().x, mgr->getWindowSize().y));

	renderer.init(*manager);

	levelView.setCenter(mgr->getWindowSize().x/2, mgr->getWindowSize().y / 2);
	levelView.setSize(mgr->getWindowSize().x, mgr->getWindowSize().y);

	playerID = 0;

	cc.setFillColor(sf::Color::Green);
	cc.setRadius(20);
	cc.setPosition(10,10);

	chat.init(manager->getAssets(), 20, manager->getWindowSize().y - 10);
}

void PlayState::init() {
	loaded = false;

	if (socket.connect(sf::IpAddress::LocalHost, Constants::GAME_PORT) == sf::Socket::Status::Done) {
		status.setString("Connected to server! Waiting for handshake...");
	}

	_LOG_.log("PlayState", "Ready.");
}

void PlayState::render(sf::RenderWindow& window) {
	if (!loaded) {
		window.draw(status);
		return;
	}

	window.setView(levelView);

	chat.render(window);
	
	for (int i = 0; i < game.players.size(); i++) {
		Player& pl = game.players[i];

		window.setView(playerView);

		window.draw(cc);

		renderer.drawPlayer(pl);

		window.setView(window.getDefaultView());
	}
}

void PlayState::input(sf::Event ev) {
	if (ev.type == sf::Event::KeyPressed) {
		if (ev.key.code == sf::Keyboard::W) socket.send(GameProtocol::clientMove(sf::Vector2f(0, -1)));
		if (ev.key.code == sf::Keyboard::A) socket.send(GameProtocol::clientMove(sf::Vector2f(-1, 0)));
		if (ev.key.code == sf::Keyboard::S) socket.send(GameProtocol::clientMove(sf::Vector2f(0, 1)));
		if (ev.key.code == sf::Keyboard::D) socket.send(GameProtocol::clientMove(sf::Vector2f(1, 0)));
	}


	if (ev.type == sf::Event::KeyReleased) {
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W)
			&& !sf::Keyboard::isKeyPressed(sf::Keyboard::A)
			&& !sf::Keyboard::isKeyPressed(sf::Keyboard::S)
			&& !sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			socket.send(GameProtocol::clientMove(sf::Vector2f(0, 0)));
		}
			
	}
}

void PlayState::update() {
	sf::Packet packet;

	if (socket.getRemoteAddress() != sf::IpAddress::None) if (socket.receive(packet) == sf::Socket::Status::Done) {
		processPacket(packet);
	}
}

void PlayState::processNetwork() {
	sf::Packet packet;
	if (socket.receive(packet) == sf::Socket::Status::Done) {
		processPacket(packet);
	}
}

void PlayState::processPacket(sf::Packet & packet) {
	if (!loaded) {
		if (!GameProtocol::verifyServerHandshake(packet)) {
			status.setString("Wrong server!");
		} else {
			status.setString("Server verified. Confirming handshake...");

			GameProtocol::sendClientHandshake(socket, "MrOnlineCoder");
			loaded = true;
		}

		return;
	}

	NetMessage netmsg;
	packet >> netmsg;

	if (netmsg == NetMessage::SV_KICKED) {
		std::string msg;
		packet >> msg;

		socket.disconnect();

		status.setString("Kicked: "+msg);
		chat.addMessage(sf::Color::Red, "You were kicked from the server: "+msg);
		return;
	}

	if (netmsg == NetMessage::SV_LOADLEVEL) {
		std::string lvl;
		packet >> lvl;
		status.setString("Loading level '"+lvl+"'");
		return;
	}

	if (netmsg == NetMessage::SV_PLAYERIDENTIFY) {
		int id;
		packet >> id;
		playerID = id;

		manager->getAssets().playSound("lol");

		_LOG_.log("Client", "Set own player ID to "+std::to_string(playerID));
	}

	if (netmsg == NetMessage::SV_ADDPLAYER) {
		std::string name;
		packet >> name;

		chat.addMessage(sf::Color::White, name+" joined the game.");

		game.addPlayer(name);
		return;
	}

	if (netmsg == NetMessage::SV_PLAYERCHANGE) {
		int id;

		packet >> id;
		packet >> game.players[id];

		playerView.setCenter(renderer.getPlayerCenter(game.players[playerID].pos));

		return;
	}
}
