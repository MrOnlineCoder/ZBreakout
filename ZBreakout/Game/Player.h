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
#include "Inventory.h"
#include "Weapon.h"

class Player {
public:
	Player(std::string nickname);

	const std::string& getNickname();

	sf::Vector2f pos;
	sf::Vector2f accel;
	
	int direction;
	int hp;
	int currentSlot;

	//for game/server only
	bool dirty;
	
	// inventory
	Item items[Constants::PLAYER_SLOTS];
	Weapon weapons[Constants::PLAYER_SLOTS];

	void setEmptySlot(int index);
	void setWeapon(int index, std::string type);
	bool isSlotFree(int index);
	bool hasEmptySlots();
	int getEmptySlot();
	int getEmptyOrSelectedSlot();
	float getWatchAngle();

	Weapon& getWeapon();
	Item& getItem();
private:
	std::string nickname;
};

#endif
