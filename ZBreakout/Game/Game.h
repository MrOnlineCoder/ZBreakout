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

#include <SFML/Network.hpp>

#include "../Logger.h"
#include "Constants.h"

#include "Player.h"

class Game {
public:
	void addPlayer(std::string name);

	std::vector<Player> players;

	void tick();
};

#endif