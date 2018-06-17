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
	lvlLoadThread(&PlayState::loadThreadFunc, this) {

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

	chat.init(manager->getAssets(), 20, manager->getWindowSize().y - 10);

	debugTxt.setFont(manager->getAssets().getFont("main"));
	debugTxt.setPosition(25, 25);
	debugTxt.setCharacterSize(18);
	debugTxt.setFillColor(sf::Color::White);

	debug = false;

	serverIP = sf::IpAddress("127.0.0.1");
	port = Constants::CLIENT_PORT;
}

void PlayState::init() {
	loaded = false;
	debug = false;

	if (socket.bind(Constants::CLIENT_PORT) == sf::Socket::Done) {
		status.setString("UDP socket created, sending handshake...");

		sf::Packet packet;
		packet << NetMessage::CL_HANDSHAKE << "MrOnlineCoder" << Constants::VERSION;
		socket.send(packet, serverIP, Constants::SERVER_PORT);
	}
	
	_LOG_.log("PlayState", "Ready.");
}

void PlayState::render(sf::RenderWindow& window) {
	if (!loaded) {
		window.draw(status);
		return;
	}
	window.clear(sf::Color(0, 100, 0));
	
	window.setView(playerView);

	for (int i = 0; i < game.bullets.size(); i++) {
		Bullet& b = game.bullets[i];

		renderer.drawBullet(b);
	} 

	renderer.drawLevel();

	for (int i = 0; i < game.zombies.size(); i++) {
		Zombie& z = game.zombies[i];

		renderer.drawZombie(z);
	}

	for (int i = 0; i < game.players.size(); i++) {
		Player& pl = game.players[i];

		renderer.drawPlayer(pl);
	}

	renderer.renderDamageHits();

	window.setView(guiView);
	
	chat.render(window);

	renderer.drawPlayerHPBar(game.players[playerID]);
	renderer.drawInventory(game.players[playerID]);

	if (debug) {
		window.setView(playerView);
		renderer.drawDebug();

		window.setView(guiView);
		window.draw(debugTxt);
	}

	fps.update();
}

void PlayState::input(sf::Event ev) {
	if (ev.type == sf::Event::KeyPressed) {
		if (ev.key.code == sf::Keyboard::W) socket.send(GameProtocol::clientMove(sf::Vector2f(0, -1)), serverIP, Constants::SERVER_PORT);
		if (ev.key.code == sf::Keyboard::A) socket.send(GameProtocol::clientMove(sf::Vector2f(-1, 0)), serverIP, Constants::SERVER_PORT);
		if (ev.key.code == sf::Keyboard::S) socket.send(GameProtocol::clientMove(sf::Vector2f(0, 1)), serverIP, Constants::SERVER_PORT);
		if (ev.key.code == sf::Keyboard::D) socket.send(GameProtocol::clientMove(sf::Vector2f(1, 0)), serverIP, Constants::SERVER_PORT);

		if (ev.key.code == sf::Keyboard::Num1) setPlayerCurrentSlot(0);
		if (ev.key.code == sf::Keyboard::Num2) setPlayerCurrentSlot(1);
		if (ev.key.code == sf::Keyboard::Num3) setPlayerCurrentSlot(2);
		if (ev.key.code == sf::Keyboard::Num4) setPlayerCurrentSlot(3);
		if (ev.key.code == sf::Keyboard::Num5) setPlayerCurrentSlot(4);

		if (ev.key.code == sf::Keyboard::K) {
			sf::Packet p;
			p << NetMessage::CL_BUYWEAPON << Weapon::AK47;
			socket.send(p, serverIP, Constants::SERVER_PORT);

			p.clear();
			p << NetMessage::CL_BUYWEAPON << Weapon::SHOTGUN;
			socket.send(p, serverIP, Constants::SERVER_PORT);

			p.clear();
			p << NetMessage::CL_BUYWEAPON << Weapon::REVOLVER;
			socket.send(p, serverIP, Constants::SERVER_PORT);

			p.clear();
			p << NetMessage::CL_BUYWEAPON << Weapon::PISTOL;
			socket.send(p, serverIP, Constants::SERVER_PORT);
			return;
		}

		if (ev.key.code == sf::Keyboard::L) {
			sf::Packet p;
			p << NetMessage::DBG_MOVEZOMBIES;
			socket.send(p, serverIP, Constants::SERVER_PORT);
			chat.addMessage(sf::Color::Yellow, "Moved all zombies");
			return;
		}

		if (ev.key.code == sf::Keyboard::P) {
			sf::Packet p;
			p << NetMessage::DBG_SPAWNZOMBIE;
			socket.send(p, serverIP, Constants::SERVER_PORT);

			chat.addMessage(sf::Color::Yellow, "Spawned debug zombie");
			return;
		}
	}


	if (ev.type == sf::Event::KeyReleased) {
		if (ev.key.code == sf::Keyboard::F3) {
			debug = !debug;
			renderer.debug.enabled = debug;
			return;
		}


		//Player movement
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W)
			&& !sf::Keyboard::isKeyPressed(sf::Keyboard::A)
			&& !sf::Keyboard::isKeyPressed(sf::Keyboard::S)
			&& !sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			socket.send(GameProtocol::clientMove(sf::Vector2f(0, 0)), serverIP, Constants::SERVER_PORT);
		}
			
	}

	if (ev.type == sf::Event::MouseButtonReleased) {
		if (ev.mouseButton.button == sf::Mouse::Left) {
			shoot();
		}
	}
}

void PlayState::update() {
	clock.restart();

	sf::Packet udpPacket;
	sf::IpAddress ip;

	int _packet = 0;
	int _netDataSize = 0;

	while (socket.receive(udpPacket, ip, port) == sf::Socket::Status::Done) {
		if (port == Constants::SERVER_PORT) {
			_packet++;
			_netDataSize += udpPacket.getDataSize();
			processPacket(udpPacket);
		}
	}

	if (_packet != 0) packets = _packet;
	if (_netDataSize != 0) netDataSize = _netDataSize;

	
	//if we were identfied, so probably we are in game and everything is OK
	if (playerID != -1 && game.players.size() > playerID) {
		Player& me = game.players[playerID];
		
		game.moveBullets();

		if (me.isHoldingWeapon()) {
			if (me.getWeapon().getAmmo() == 0) {
				if (me.getWeapon().reloadClock.getElapsedTime().asSeconds() > me.getWeapon().getReloadTime()) {
					me.getWeapon().ammo = me.getWeapon().getMaxAmmo();
				}

				renderer.updateItemText(me);
			} else {
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) if (me.getWeapon().isAuto()) {
					if (me.getWeapon().fireClock.getElapsedTime().asSeconds() > me.getWeapon().getFireDelay()) {
						me.getWeapon().fireClock.restart();
						shoot();
					}
				}
			}
		}
	}



	if (debug) {
		debugStream.str("");

		debugStream << "FPS: " << fps.getFPS() << "\n";
		debugStream << "Tickrate: " << Constants::GAME_TICKRATE << "\n";
		debugStream << "Version: " << Constants::VERSION << "\n";
		debugStream << "Zombies: " << game.zombies.size() << "\n";
		debugStream << "Bullets: " << game.bullets.size() << "\n";
		debugStream << "Client / Server port: " << Constants::CLIENT_PORT << " / " << Constants::SERVER_PORT << "\n";
		debugStream << "Level: " << levelName << "\n";
		debugStream << "Packets processed: " << packets << " and " << netDataSize << " bytes\n";

		debugTxt.setString(debugStream.str());
	}
}

void PlayState::loadThreadFunc() {
	lvl.loadFromFile(ASSETS_PATH + "/levels/" + levelName + ".tmx");

	status.setString("Creating pathsearch grid..");
	Pathsearch::rasterizeGrid(lvl.getWalls(), sf::Vector2f(manager->getAssets().getTexture("zombie_idle").getSize()));

	playerView.setCenter(renderer.getPlayerCenter(game.players[playerID].pos));
	loaded = true;

	renderer.setLevel(lvl.getTMXMap());
	renderer.debug.walls = &lvl.getWalls();
}


void PlayState::processPacket(sf::Packet & packet) {
	NetMessage netmsg;
	packet >> netmsg;

	if (netmsg == NetMessage::SV_KICKED) {
		std::string msg;
		packet >> msg;

		socket.unbind();

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

		//manager->getAssets().playSound("lol");

		_LOG_.log("Client", "Set own player ID to " + std::to_string(playerID));
		return;
	}

	if (netmsg == NetMessage::SV_ADDPLAYER) {
		std::string name;
		packet >> name;

		chat.addMessage(sf::Color::White, name+" joined the game.");

		game.addPlayer(name);
		game.players[game.players.size() - 1].pos = lvl.getStartPosition();
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

		return;
	}

	if (netmsg == NetMessage::SV_AMMOCHANGE) {
		Player& me = game.players[playerID];

		me.getWeapon().ammo--;

		if (me.getWeapon().ammo == 0) {
			me.getWeapon().reloadClock.restart();
			manager->getAssets().playSound("weapon_reload");
		}

		renderer.updateItemText(me);
		return;
	}

	if (netmsg == NetMessage::SV_ADDZOMBIE) {
		ZombieType type;
		sf::Vector2f pos;

		packet >> type;
		packet >> pos;
		
		game.addZombie(type, pos);
		return;
	}

	if (netmsg == NetMessage::SV_CHANGEZOMBIE) {
		int index;
		Zombie zombie = Zombie::createZombie(ZombieType::NORMAL);
		
		packet >> index;
		packet >> zombie;

		if (index >= game.zombies.size()) return;

		Zombie& old = game.zombies[index];

		if (old.hp > zombie.hp) {
			int dmg = old.hp - zombie.hp; //dmg is the difference between old and new hp, so - the real damage

			manager->getAssets().playSound("zombie_hit");

			renderer.playDamageEffect(old.pos, dmg);
		}

		game.zombies[index] = zombie;
		return;
	}

	if (netmsg == NetMessage::SV_KILLZOMBIE) {
		int index;

		packet >> index;

		manager->getAssets().playSound("zombie_death");

		game.zombies.erase(game.zombies.begin() + index);
		return;
	}
}

void PlayState::setPlayerCurrentSlot(int slot) {
	game.players[playerID].currentSlot = slot;

	sf::Packet packet;
	packet << NetMessage::CL_SLOTCHANGE << slot;

	socket.send(packet, serverIP, Constants::SERVER_PORT);

	renderer.updateItemText(game.players[playerID]);
}

void PlayState::shoot() {
	if (game.players[playerID].isSlotFree(game.players[playerID].currentSlot)) return;

	Player& me = game.players[playerID];

	if (!me.isHoldingWeapon()) return;

	if (me.getWeapon().getAmmo() > 0) {
		manager->getAssets().playSound(Weapon::getWeaponShotSoundPath(me.getWeapon().getType()));
	}

	sf::Packet packet;
	packet << NetMessage::CL_SHOOT;

	socket.send(packet, serverIP, Constants::SERVER_PORT);
}
