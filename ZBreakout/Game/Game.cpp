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

Game::Game() {
	nospawn = false;
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

void Game::tick() {
	for (std::vector<Player>::size_type plid = 0; plid < players.size(); plid++) {
		Player& player = players[plid];

		if (player.accel.x == 0 && player.accel.y == 0) continue;

		if (!canPlayerMove(plid)) {
			continue;
		}

		player.pos += player.accel;
		player.dirty = true;

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

			z.pos.x += 1.0f;
			z.dirty = true;

			++zi;
		} else{
			zi = zombies.erase(zi);
		}
	}

	if (!nospawn && spawnClock.getElapsedTime().asSeconds() > 5.0f) {
		spawnClock.restart();

		for (int n = 0; n < 1; n++) spawnZombie();
		nospawn = true;
	}
}

bool Game::canPlayerMove(PlayerID index) {
	playerAABB.left = players[index].pos.x - playerAABB.width/2 + players[index].accel.x;
	playerAABB.top = players[index].pos.y - playerAABB.height/2 + players[index].accel.y;

	return !level.collide(playerAABB);
}

int Game::findBulletDamageToZombie(Zombie& z) {
	zombieAABB.left = z.pos.x;
	zombieAABB.top = z.pos.y;

	int damage = 0;


	std::vector<Bullet>::iterator bi = bullets.begin();
	while (bi != bullets.end()) {

		Bullet& b = *bi;
		if (zombieAABB.contains(b.pos)) {
			damage += b.damage;
			bi = bullets.erase(bi);
		}
		else {
			++bi;
		}
	}

	return damage;
}

void Game::spawnZombie() {
	addZombie(ZombieType::NORMAL, level.getSpawners().at(Random::randomInt(0, level.getSpawners().size()-1)));
}

void Game::addZombie(ZombieType type, sf::Vector2f pos) {
	Zombie z = Zombie::createZombie(type);
	z.pos = pos;
	zombies.push_back(z);

	_LOG_.log("MicroslavikSpawn", "Spawned microslavik ID="+std::to_string(static_cast<int>(type))+" at position "+std::to_string(pos.x)+", "+std::to_string(pos.y));
}
