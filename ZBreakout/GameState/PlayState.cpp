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
	: GameState(mgr),
	lvlLoadThread(&PlayState::loadThreadFunc, this){

	status.setFont(manager->getAssets().getFont("main"));
	status.setPosition(25, manager->getWindowSize().y - status.getCharacterSize() - 25);
	status.setString("Connecting to server...");
	status.setFillColor(sf::Color::White);

	socket.setBlocking(false);

	playerView.setSize(sf::Vector2f(mgr->getWindowSize().x, mgr->getWindowSize().y));

	renderer.init(*manager);

	guiView.setCenter(mgr->getWindowSize().x/2, mgr->getWindowSize().y / 2);
	guiView.setSize(mgr->getWindowSize().x, mgr->getWindowSize().y);

	playerID = -1;

	cc.setFillColor(sf::Color::Green);
	cc.setRadius(20);
	cc.setPosition(10,10);

	chat.init(manager->getAssets(), 20, manager->getWindowSize().y - 10);

	debugTxt.setFont(manager->getAssets().getFont("main"));
	debugTxt.setPosition(25, 25);
	debugTxt.setCharacterSize(18);
	debugTxt.setFillColor(sf::Color::White);

	debug = false;
}

void PlayState::init() {
	loaded = false;
	debug = false;

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
	
	window.setView(playerView);

	lvl.render(window);

	for (int i = 0; i < game.bullets.size(); i++) {
		Bullet& b = game.bullets[i];

		renderer.drawBullet(b);
	}

	for (int i = 0; i < game.players.size(); i++) {
		Player& pl = game.players[i];

		window.draw(cc);

		renderer.drawPlayer(pl);

		window.setView(window.getDefaultView());
	}

	window.setView(guiView);
	
	chat.render(window);

	renderer.drawPlayerHPBar(game.players[playerID]);
	renderer.drawInventory(game.players[playerID]);

	if (debug) window.draw(debugTxt);

	fps.update();
}

void PlayState::input(sf::Event ev) {
	if (ev.type == sf::Event::KeyPressed) {
		if (ev.key.code == sf::Keyboard::W) socket.send(GameProtocol::clientMove(sf::Vector2f(0, -1)));
		if (ev.key.code == sf::Keyboard::A) socket.send(GameProtocol::clientMove(sf::Vector2f(-1, 0)));
		if (ev.key.code == sf::Keyboard::S) socket.send(GameProtocol::clientMove(sf::Vector2f(0, 1)));
		if (ev.key.code == sf::Keyboard::D) socket.send(GameProtocol::clientMove(sf::Vector2f(1, 0)));

		if (ev.key.code == sf::Keyboard::Num1) setPlayerCurrentSlot(0);
		if (ev.key.code == sf::Keyboard::Num2) setPlayerCurrentSlot(1);
		if (ev.key.code == sf::Keyboard::Num3) setPlayerCurrentSlot(2);
		if (ev.key.code == sf::Keyboard::Num4) setPlayerCurrentSlot(3);
		if (ev.key.code == sf::Keyboard::Num5) setPlayerCurrentSlot(4);

		if (ev.key.code == sf::Keyboard::J) {
			sf::Packet p;
			p << NetMessage::CL_BUYWEAPON << Weapon::PISTOL;
			socket.send(p);
		}

		if (ev.key.code == sf::Keyboard::K) {
			sf::Packet p;
			p << NetMessage::CL_BUYWEAPON << Weapon::REVOLVER;
			socket.send(p);
		}
	}


	if (ev.type == sf::Event::KeyReleased) {
		if (ev.key.code == sf::Keyboard::F3) {
			debug = !debug;
			return;
		}


		//Player movement
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W)
			&& !sf::Keyboard::isKeyPressed(sf::Keyboard::A)
			&& !sf::Keyboard::isKeyPressed(sf::Keyboard::S)
			&& !sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			socket.send(GameProtocol::clientMove(sf::Vector2f(0, 0)));
		}
			
	}

	if (ev.type == sf::Event::MouseButtonReleased) {
		if (ev.mouseButton.button == sf::Mouse::Left) {
			if (game.players[playerID].isSlotFree(game.players[playerID].currentSlot)) return;

			sf::Packet packet;
			packet << NetMessage::CL_SHOOT;

			socket.send(packet);
		}
	}
}

void PlayState::update() {
	sf::Packet packet;

	if (socket.getRemoteAddress() != sf::IpAddress::None) if (socket.receive(packet) == sf::Socket::Status::Done) {
		processPacket(packet);
	}

	
	//if we were identfied, so probably we are in game and everything is OK
	if (playerID != -1 && game.players.size() > playerID) {
		Player& me = game.players[playerID];
		
		game.moveBullets();

		if (me.getWeapon().getAmmo() == 0) {
			if (me.getWeapon().reloadClock.getElapsedTime().asSeconds() > me.getWeapon().getReloadTime()) {
				me.getWeapon().ammo = me.getWeapon().getMaxAmmo();
				renderer.updateItemText(me);
			}
		}
	}

	if (debug) {
		debugStream.str("");

		debugStream << "FPS: " << fps.getFPS() << "\n";
		debugStream << "Tickrate: " << Constants::GAME_TICKRATE << "\n";
		debugStream << "Version: " << Constants::VERSION << "\n";
		debugStream << "currentSlot: " << game.players[playerID].currentSlot << "\n";

		debugTxt.setString(debugStream.str());
	}
}

void PlayState::loadThreadFunc() {
	lvl.loadFromFile(ASSETS_PATH + "/levels/" + levelName + ".tmx");
	playerView.setCenter(renderer.getPlayerCenter(game.players[playerID].pos));
	loaded = true;
}

void PlayState::processNetwork() {
	sf::Packet packet;
	if (socket.receive(packet) == sf::Socket::Status::Done) {
		processPacket(packet);
	}
}

void PlayState::processPacket(sf::Packet & packet) {
	NetMessage netmsg;
	packet >> netmsg;

	if (netmsg == NetMessage::SV_HANDSHAKE) {
		if (!GameProtocol::verifyServerHandshake(packet)) {
			status.setString("Cannot connect to the server: invalid handshake.");
			socket.disconnect();
			_LOG_.log("Client", "Server has wrong handshake signature.");
		}
		else {
			status.setString("Server verified. Confirming handshake...");
			_LOG_.log("Client", "Valid server, sending own handshake...");

			GameProtocol::sendClientHandshake(socket, "MrOnlineCoder");
		}

		return;
	}

	if (netmsg == NetMessage::SV_KICKED) {
		std::string msg;
		packet >> msg;

		socket.disconnect();

		status.setString("Kicked: "+msg);
		chat.addMessage(sf::Color::Red, "You were kicked from the server: "+msg);
		return;
	}

	if (netmsg == NetMessage::SV_LOADLEVEL) {
		packet >> levelName;
		status.setString("Loading level '"+ levelName +"'");
		lvlLoadThread.launch();
		return;
	}

	if (netmsg == NetMessage::SV_PLAYERIDENTIFY) {
		int id;
		packet >> id;
		playerID = id;

		manager->getAssets().playSound("lol");

		_LOG_.log("Client", "Set own player ID to " + std::to_string(playerID));
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

	if (netmsg == NetMessage::SV_ADDWEAPON) {
		std::string type;
		packet >> type;

		Player& pl = game.players[playerID];

		pl.setWeapon(pl.getEmptyOrSelectedSlot(), type);
		chat.addMessage(sf::Color::Magenta, "You picked up "+Weapon::getWeaponName(type));

		renderer.updateItemText(game.players[playerID]);
		return;
	}

	if (netmsg == NetMessage::SV_SHOTMADE) {
		Bullet b;
		packet >> b;

		game.addBullet(b);

		if (b.shooter == playerID) {
			Player& me = game.players[playerID];

			me.getWeapon().ammo--;
			
			if (me.getWeapon().ammo == 0) {
				me.getWeapon().reloadClock.restart();
				manager->getAssets().playSound("weapon_reload");
			}

			renderer.updateItemText(me);
		}

		manager->getAssets().playSound(Weapon::getWeaponShotSoundPath(b.type));
		return;
	}
}

void PlayState::setPlayerCurrentSlot(int slot) {
	game.players[playerID].currentSlot = slot;

	sf::Packet packet;
	packet << NetMessage::CL_SLOTCHANGE << slot;

	socket.send(packet);

	renderer.updateItemText(game.players[playerID]);
}
