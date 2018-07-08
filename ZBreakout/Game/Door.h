/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#ifndef DOOR_H
#define DOOR_H

#include <SFML/Graphics/Rect.hpp>
#include <string>

struct Door {
	std::string name;
	sf::FloatRect bounds;
	int price;

	int index;
};

#endif