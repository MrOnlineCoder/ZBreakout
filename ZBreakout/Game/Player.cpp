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

	//Set default data
	pos = sf::Vector2f(50.0f,50.0f);
	dirty = false;
	accel = sf::Vector2f(0.0f,0.0f);
	direction = 0;
	hp = Constants::MAX_PLAYER_HP;
	currentSlot = 0;
	gold = 0;

	//Clear inventory
	for (int i = 0; i < Constants::PLAYER_SLOTS; i++) {
		setEmptySlot(i);
	}
}


const std::string & Player::getNickname() {
	return nickname;
}

void Player::setEmptySlot(int index) {
	items[index].type = ItemType::EMPTY;
	items[index].data = 0;
}

void Player::setWeapon(int index, std::string type) {
	items[index].type = ItemType::WEAPON;
	items[index].data = index;

	weapons[index] = Weapon::createWeapon(type);
}

bool Player::isSlotFree(int index) {
	return items[index].type == ItemType::EMPTY;
}

bool Player::hasEmptySlots() {
	for (int i = 0; i < Constants::PLAYER_SLOTS; i++) {
		if (isSlotFree(i)) return true;
	}

	return false;
}

int Player::getEmptySlot() {
	for (int i = 0; i < Constants::PLAYER_SLOTS; i++) {
		if (isSlotFree(i)) return i;
	}

	return -1; //check for -1 required to avoid index out of bounds exception
}

int Player::getEmptyOrSelectedSlot() {
	int es = getEmptySlot();

	if (es == -1) {
		return currentSlot;
	} else {
		return es;
	}
}

float Player::getWatchAngle() {
	return 0.0f;
}

bool Player::isHoldingWeapon() {
	return items[currentSlot].type == ItemType::WEAPON;
}

Weapon & Player::getWeapon() {
	return weapons[currentSlot];
}

Item & Player::getItem() {
	return items[currentSlot];
}
