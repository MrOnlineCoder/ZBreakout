/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#include "Player.h"

Player::Player(std::string nickname) {
	this->nickname = nickname;

	pos = sf::Vector2f(50.0f,50.0f);
	dirty = false;
	accel = sf::Vector2f(0.0f,0.0f);
	direction = 0;
}


const std::string & Player::getNickname()
{
	return nickname;
}
