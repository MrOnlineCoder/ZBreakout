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

void Game::tick() {
	for (std::vector<Player>::size_type plid = 0; plid < players.size(); plid++) {
		Player& player = players[plid];

		if (player.accel.x == 0 && player.accel.y == 0) return;

		player.pos += player.accel;

		player.dirty = true;
	}
}