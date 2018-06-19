/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#ifndef GAME_H
#define GAME_H

#include <queue>

#include <SFML/Network.hpp>

#include "../Logger.h"
#include "Constants.h"


#include "Player.h"
#include "Level.h"
#include "Zombie.h"

enum class GameEventType {
	AddGold
};

struct GameEvent {
	GameEventType type;
	union {
		int goldGiven;
	};
	PlayerID player;
};

/*
	Game class
	Contains a lot of game-related data and methods for manipulation.

	Present both on client and the server, should be synced and supported in up-to-date state.
	On client is used only as source for rendering input
	On server as main game instance.
	
	tick() should be called each GameServer tick, so game events will happen.
*/
class Game {
public:
	Game();

	void setPlayerSize(sf::Vector2f arg);
	void setZombieSize(sf::Vector2f size);

	void addPlayer(std::string name);
	void addBullet(Bullet bullet);

	void addZombie(ZombieType type, sf::Vector2f pos);

	void spawnZombie();

	void updatePlayerAttackers(PlayerID id);

	//also used by client
	void moveBullets();
	void moveZombies();

	std::vector<Player> players;
	std::vector<Bullet> bullets;
	std::vector<Zombie> zombies;

	void tick();

	Level level;

	float delta;
	bool isServer;

	bool shouldMoveZombies;

	std::queue<GameEvent> eventQueue;
private:
	//used for collision detection
	sf::Vector2f playerSize;
	sf::FloatRect playerAABB;

	sf::FloatRect zombieAABB;
	bool canPlayerMove(PlayerID index);
	int findBulletDamageToZombie(Zombie& z);
	//

	void moveZombie(Zombie& z);

	PlayerID findNearestPlayerTo(sf::Vector2f pos);

	int getZombieReward(Zombie& z, Player& pl);

	sf::Clock spawnClock;
	sf::Clock gameClock;
	bool nospawn;
};

#endif