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

	//public positions and acceleration
	sf::Vector2f pos;
	sf::Vector2f accel;
	
	//Direction in which player is looking
	//depends on the texture
	//direction in degrees = direction * 90
	int direction;

	//Health
	int hp;

	//Current inventory slot
	int currentSlot;

	//For game/server only
	bool dirty;
	
	//Inventory

	//This is all items
	Item items[Constants::PLAYER_SLOTS];

	//Possible weapons, items array store index of weapon here.
	Weapon weapons[Constants::PLAYER_SLOTS];

	//Sets slot #`index` as empty
	void setEmptySlot(int index);

	//Sets weapon to slot #index
	void setWeapon(int index, std::string type);

	bool isSlotFree(int index);

	bool hasEmptySlots();

	int getEmptySlot();

	int getEmptyOrSelectedSlot();

	//no-use
	float getWatchAngle();

	bool isHoldingWeapon();

	Weapon& getWeapon(); //use only after isHoldingWeapon() check
	Item& getItem();
private:
	std::string nickname;
};

#endif
