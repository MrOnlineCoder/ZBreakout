/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#ifndef INVENTORY_H
#define INVENTORY_H

#include <SFML/Graphics.hpp>

#include "../Logger.h"
#include "Constants.h"

enum class ItemType {
	EMPTY,
	WEAPON,
	MINE,
	MEDKIT
};

struct Item {
	ItemType type;
	int data; //for weapons it is index in players' weapons array, for other items - amount / quantity
};

#endif
