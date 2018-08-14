/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#include "Game.h"
#include "Random.h"
#include "GameMath.h"

GameEvent EventFactory::errorEvent(PlayerID pl, std::string err) {
	GameEvent evt;
	evt.player = pl;
	evt.error = err;
	evt.type = GameEventType::SendError;
	return evt;
}

GameEvent EventFactory::giveGoldEvent(PlayerID pl, int gold) {
	GameEvent evt;
	evt.player = pl;
	evt.goldGiven = gold;
	evt.type = GameEventType::AddGold;
	return evt;
}

GameEvent EventFactory::openDoorEvent(PlayerID pl, int doorID) {
	GameEvent evt;
	evt.player = pl;
	evt.door = doorID;
	evt.type = GameEventType::OpenDoor;
	return evt;
}


Game::Game() {
	nospawn = false;

	zombies.reserve(150);
	players.reserve(4);
	bullets.reserve(300);

	delta = 1.0f;
	shouldMoveZombies = false;
}

void Game::setPlayerSize(sf::Vector2f arg) {
	playerSize = arg;

	playerAABB.width = arg.x;
	playerAABB.height = arg.y;

	spawnClock.restart();
}

void Game::setZombieSize(sf::Vector2f size) {
	zombieAABB.width = size.x;
	zombieAABB.height = size.y;
}

void Game::addPlayer(std::string name) {
	players.push_back(Player(name));
}

void Game::addBullet(Bullet bullet) {
	bullets.push_back(bullet);
}

void Game::moveBullets() {
	std::vector<Bullet>::iterator it;
	for (it = bullets.begin(); it != bullets.end();) {
		Bullet bullet = *it;
		float angle = bullet.angle;

		it->pos.x += Constants::BULLET_SPEED * -std::sin(3.1415 / 180.0f * (360.0f - angle - 90));
		it->pos.y += Constants::BULLET_SPEED * -std::cos(3.1415 / 180.0f * (360.0f - angle - 90));

		if (bullet.pos.x < 0 || bullet.pos.y < 0 || bullet.pos.y > Constants::LEVEL_SIZE || bullet.pos.x > Constants::LEVEL_SIZE) {
			it = bullets.erase(it);
		} else {
			++it;
		}
	}
}

void Game::moveZombies() {
	for (int i = 0; i < zombies.size(); i++) {
		moveZombie(zombies[i]);
	}
}

void Game::tick() {
	for (std::vector<Player>::size_type plid = 0; plid < players.size(); plid++) {
		Player& player = players[plid];

		if (player.accel.x == 0 && player.accel.y == 0) continue;

		if (!canPlayerMove(plid)) {
			continue;
		}

		player.pos += player.accel;
		player.dirty = true;
		updatePlayerAttackers(plid);
	}

	moveBullets();

	std::vector<Zombie>::iterator zi = zombies.begin();
	while (zi != zombies.end()) {

		Zombie& z = *zi;

		if (z.isAlive()) {
			//zombie tick

			int possibleDmg = findBulletDamageToZombie(z);

			if (possibleDmg > 0) {

				z.hp -= possibleDmg;
				z.dirty = true;

			}

			if (shouldMoveZombies) {
				moveZombie(z);
			}

			++zi;
		} else{
			if (isServer) zi = zombies.erase(zi);
		}
	}
}

bool Game::canPlayerMove(PlayerID index) {
	playerAABB.left = players[index].pos.x - playerAABB.width/2 + players[index].accel.x;
	playerAABB.top = players[index].pos.y - playerAABB.height/2 + players[index].accel.y;

	return !level.collide(playerAABB);
}

int Game::findBulletDamageToZombie(Zombie& z) {
	//we pad original position, client uses setOrigin(half of width, half of height) for better rendering
	//Renderer.cpp:line122
	zombieAABB.left = z.pos.x - zombieAABB.width / 2;
	zombieAABB.top = z.pos.y - zombieAABB.height / 2;

	int damage = 0;

	std::vector<Bullet>::iterator bi = bullets.begin();
	bool zdead = false;
	while (bi != bullets.end()) {

		Bullet& b = *bi;

		if (zombieAABB.contains(b.pos)) {
			damage += b.damage;

			if (!zdead && z.hp - b.damage <= 0) {
				//reward player who final shot this zombie
				modifyPlayerGold(b.shooter, getZombieReward(z, players[b.shooter]));

				zdead = true;
			}

			bi = bullets.erase(bi);
		} else {
			++bi;
		}
	}


	return damage;
}

void Game::moveZombie(Zombie & z) {
	if (z.path.current == z.path.end) return;

	if (GameMath::distanceSquared(z.pos, z.nextStep) <= 5 * 5) {
		sf::Vector2i node = Pathsearch::searchNextNode(z.path);
		z.nextStep = Pathsearch::mapNode(node);
		z.velocity = GameMath::findVelocity(z.pos, z.nextStep, z.getMoveSpeed());
	} else {
		z.pos += z.velocity * delta;
		z.dirty = true;
	}
}

PlayerID Game::findNearestPlayerTo(sf::Vector2f pos) {
	PlayerID min = 0;

	float dist = GameMath::distance(pos, players[0].pos);

	for (int i = 0; i < players.size(); i++) {
		sf::Vector2f ppos = players[i].pos;

		float _dist = GameMath::distance(pos, ppos);
		if (_dist < dist) {
			dist = _dist;
			min = i;
		}
	}

	return min;
}

int Game::getZombieReward(Zombie & z, Player & pl) {
	int reward = 0;

	if (pl.getWeapon().getType() == Weapon::SHOTGUN) {
		reward = 2;
	}

	if (pl.getWeapon().getType() == Weapon::AK47) {
		reward = 5;
	}

	if (pl.getWeapon().getType() == Weapon::PISTOL) {
		reward = 5;
	}

	if (pl.getWeapon().getType() == Weapon::REVOLVER) {
		reward = 2;
	}

	return reward;
}

void Game::modifyPlayerGold(PlayerID id, int delta) {
	eventQueue.push(EventFactory::giveGoldEvent(id, delta));

	players[id].gold += delta;
}

void Game::updatePlayerAttackers(PlayerID id) {
	for (int i = 0; i < zombies.size(); i++) {
		Zombie& z = zombies[i];

		if (z.target == id) {
			Pathsearch::initPath(&z.path, z.pos, players[z.target].pos);
			z.nextStep = z.pos;
		}
	}
}

void Game::checkPlayerAction(PlayerID id) {
	Player& pl = players[id];

	for (auto i = 0; i < level.getDoors().size(); i++) {
		Door& door = level.getDoors()[i];

		if (door.open) continue;

		float dist = GameMath::distanceSquared(pl.pos, sf::Vector2f(door.bounds.left+door.bounds.width/2, door.bounds.top));
		
		if (dist < Constants::ACTION_DISTANCE *  Constants::ACTION_DISTANCE) {
			if (pl.gold < door.price) {
				eventQueue.push(EventFactory::errorEvent(id, "Not enough gold."));
				return;
			} 

			level.getWalls().erase(level.getWalls().begin() + door.index);

			modifyPlayerGold(id, -door.price);

			Pathsearch::openDoorSolids(door);

			eventQueue.push(EventFactory::openDoorEvent(id, i));

			door.open = true;
			return;
		}
	}
}

void Game::spawnZombie() {
	addZombie(ZombieType::NORMAL, level.getSpawners().at(
		Random::randomInt(0, level.getSpawners().size()-1)) /*+ sf::Vector2f(Random::randomInt(-50, 50), Random::randomInt(-50, 50))*/
	);
}

void Game::addZombie(ZombieType type, sf::Vector2f pos) {
	Zombie z = Zombie::createZombie(type);
	z.pos = pos;
	z.target = findNearestPlayerTo(z.pos);
	z.nextStep = z.pos;

	Pathsearch::initPath(&z.path, z.pos, players[z.target].pos);

	zombies.push_back(z);

	_LOG_.log("MicroslavikSpawn", "Spawned microslavik ID="+std::to_string(static_cast<int>(type))+" at position "+std::to_string(pos.x)+", "+std::to_string(pos.y)+", targeted at player #"+std::to_string(z.target));
}
