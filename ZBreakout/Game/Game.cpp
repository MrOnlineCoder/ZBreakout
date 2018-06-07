/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#include "Game.h"

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
		}

		else {
			++it;
		}
	}
}

void Game::tick() {
	for (std::vector<Player>::size_type plid = 0; plid < players.size(); plid++) {
		Player& player = players[plid];

		if (player.accel.x == 0 && player.accel.y == 0) return;

		player.pos += player.accel;

		player.dirty = true;
	}
}