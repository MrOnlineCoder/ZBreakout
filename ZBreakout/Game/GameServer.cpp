/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#include "GameServer.h"

GameServer::GameServer(AssetManager& assets)
: serverThread(&GameServer::loop, this),
gameAssets(assets) {
	tcpServer.setBlocking(false);

	running = false;
}

bool GameServer::start() {
	if (running) {
		_LOG_.log("GameServer", "WARN: tried to start already running game server!");
		return false;
	}

	_LOG_.log("GameServer", "Starting GameServer on port "+std::to_string(Constants::GAME_PORT));

	reset();

	_LOG_.log("GameServer", "Loading level "+Constants::LEVEL);

	game.level.loadFromFile("Assets/levels/" + Constants::LEVEL + ".tmx");
	
	game.setPlayerSize(sf::Vector2f(gameAssets.getTexture("player").getSize()));
	game.setZombieSize(sf::Vector2f(gameAssets.getTexture("zombie_idle").getSize()));

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

	_LOG_.log("GameServer", "Server stopped.");
}

void GameServer::reset() {
	if (running) {
		_LOG_.log("GameServer", "WARN: tried to reset running server, something went wrong?");
		return;
	}

	for (int i = 0; i < Constants::MAX_PLAYERS; i++) {
		sockets[i].setBlocking(false);
		sockets[i].disconnect();
	}

	connected = 0;
}

void GameServer::loop() {

	while (running) {
		if (connected < Constants::MAX_PLAYERS) if (tcpServer.accept(sockets[connected]) == sf::Socket::Status::Done) {
			_LOG_.log("GameServer", "TCP client connected, trying to add him as a player #"+std::to_string(connected));
			sf::TcpSocket& client = sockets[connected];
			GameProtocol::sendServerHandshake(client);
			connected++;
		}

		for (int user = 0; user < connected + 1; user++) {
			sf::Packet packet;
			if (sockets[user].receive(packet) == sf::Socket::Status::Done) {
				processPacket(user, packet);
			}
		}

		game.tick();

		sendGameDelta();

		sf::sleep(sf::milliseconds(1000 / Constants::GAME_TICKRATE));
	}
}

void GameServer::processPacket(int sender, sf::Packet packet) {
	NetMessage netmsg;

	packet >> netmsg;

	if (netmsg == NetMessage::CL_HANDSHAKE) {
		//verifyClientHandshake returns nickname of the player if handshake was successfull
		//empty string on fail
		std::string nick = GameProtocol::verifyClientHandshake(packet);

		_LOG_.log("GameServer", "Received nickname "+nick+" from client #"+ std::to_string(sender));
		
		if (nick.empty()) {
			_LOG_.log("GameServer", "Client failed to do a correct handshake - "+std::to_string(sender));
			kick(sender, "Invalid handshake. Probably server's version is different from yours.");
			return;
		}


		_LOG_.log("GameServer", "Handshake with "+ nick + " complete. Sending level `"+Constants::LEVEL+"`");

		for (int i = 0; i < game.players.size(); i++) {
			sockets[sender].send(GameProtocol::addPlayerPacket(game.players[i].getNickname()));
		}

		sockets[sender].send(GameProtocol::identifyPlayerPacket(sender));

		GameProtocol::sendLevel(sockets[sender], Constants::LEVEL);

		broadcast(GameProtocol::addPlayerPacket(nick));

		game.addPlayer(nick);
		game.players[game.players.size() - 1].pos = game.level.getStartPosition();
		return;
	}

	if (netmsg == NetMessage::CL_PLAYERMOVE) {
		sf::Vector2f dir;
		packet >> dir;

		//calculate direction in which player is looking
		int intdir = -1;

		if (dir.x == 1 && dir.y == 0) intdir = 0;
		if (dir.x == 0 && dir.y == 1) intdir = 1;
		if (dir.x == -1 && dir.y == 0) intdir = 2;
		if (dir.x == 0 && dir.y == -1) intdir = 3;

		//update only if needed (-1 means untouched)
		if (intdir != -1) game.players[sender].direction = intdir;

		game.players[sender].accel = sf::Vector2f(dir.x * Constants::PLAYER_SPEED, dir.y * Constants::PLAYER_SPEED);

		return;
	}

	if (netmsg == NetMessage::CL_BUYWEAPON) {
		WeaponType type;
		packet >> type;

		givePlayerWeapon(sender, type);
		return;
	}

	if (netmsg == NetMessage::CL_SLOTCHANGE) {
		int slot;
		packet >> slot;

		game.players[sender].currentSlot = slot;
		return;
	}

	if (netmsg == NetMessage::CL_SHOOT) {
		Player& pl = game.players[sender];
		if (pl.isSlotFree(pl.currentSlot)) return;

		if (pl.getItem().type != ItemType::WEAPON) return;

		Weapon& wpn = pl.getWeapon();

		if (wpn.getAmmo() == 0) {
			if (wpn.reloadClock.getElapsedTime().asSeconds() > wpn.getReloadTime()) {
				wpn.ammo = wpn.getMaxAmmo();
			} else {
				return;
			}
		}

		if (wpn.isAuto()) {
			if (wpn.fireClock.getElapsedTime().asSeconds() < wpn.getFireDelay()) return;
		}

		wpn.fireClock.restart();
		wpn.ammo--;

		if (wpn.ammo <= 0) wpn.reloadClock.restart();

		shoot(sender);
		return;
	}
}

void GameServer::kick(int who, std::string msg) {
	sf::Packet kickPacket;
	kickPacket << NetMessage::SV_KICKED << msg;

	sockets[who].send(kickPacket);

	sockets[who].disconnect();
}

void GameServer::sendGameDelta() {
	clock.restart();

	for (std::vector<Player>::size_type i = 0; i < game.players.size(); i++) {
		if (game.players[i].dirty) {
			Player& p = game.players[i];

			broadcast(GameProtocol::playerUpdate(i, p));

			printf("server move\n");

			game.players[i].dirty = false;
		}
	}

	for (std::vector<Zombie>::size_type i = 0; i < game.zombies.size(); i++) {
		Zombie& zm = game.zombies[i];

		if (zm.ticksAlive == 0) { //zombie just spawned
			sf::Packet packet;

			packet << NetMessage::SV_ADDZOMBIE << zm.getType() << zm.pos;

			broadcast(packet);
		}

		if (zm.dirty) {
			sf::Packet packet;
			packet << NetMessage::SV_CHANGEZOMBIE << i << zm;
			broadcast(packet);

			zm.dirty = false;

			if (!zm.isAlive()) {
				sf::Packet packet;
				packet << NetMessage::SV_KILLZOMBIE << i;
				broadcast(packet);
			}
		}

		zm.ticksAlive++;
	}
}

void GameServer::givePlayerWeapon(PlayerID id, std::string type) {
	game.players[id].setWeapon(game.players[id].getEmptyOrSelectedSlot(), type);

	sf::Packet packet;
	packet << NetMessage::SV_ADDWEAPON << type;

	sockets[id].send(packet);

	_LOG_.log("GameServer/givePlayerWeapon", "Given player #"+std::to_string(id)+" weapon "+type);
}

void GameServer::broadcast(sf::Packet & packet) {
	for (int i = 0; i < connected; i++) {
		sockets[i].send(packet);
	}
}

void GameServer::shoot(PlayerID who) {
	Player& pl = game.players[who];
	Weapon& wpn = pl.getWeapon();

	Bullet bullet;
	bullet.damage = wpn.getDamage();
	bullet.shooter = who;
	bullet.pos = pl.pos;
	bullet.angle = pl.direction * 90;
	bullet.type = wpn.getType();

	game.addBullet(bullet);

	sf::Packet shotPacket;
	shotPacket << NetMessage::SV_SHOTMADE << bullet;
	broadcast(shotPacket);


	//if player made a shot from shotgun, shoot more bullets:
	if (wpn.getType() == Weapon::SHOTGUN) {
		//bullet 2
		bullet.angle = pl.direction * 90 + 5;
		game.addBullet(bullet);

		sf::Packet shotPacket2;
		shotPacket2 << NetMessage::SV_SHOTMADE << bullet;
		broadcast(shotPacket2);

		//bullet 3
		bullet.angle = pl.direction * 90 - 5;
		game.addBullet(bullet);

		shotPacket2.clear();
		shotPacket2 << NetMessage::SV_SHOTMADE << bullet;
		broadcast(shotPacket2);
	}
	

	sf::Packet ammoPacket;
	ammoPacket << NetMessage::SV_AMMOCHANGE;
	sockets[who].send(ammoPacket);
}
