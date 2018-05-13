/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Network.hpp>

#include "../Logger.h"
#include "Constants.h"

class Player {
public:
	Player(std::string nickname);

	const std::string& getNickname();

	sf::Vector2f pos;
	sf::Vector2f accel;
	
	int direction;

	//for game/server only
	bool dirty;
private:
	std::string nickname;
};

#endif
