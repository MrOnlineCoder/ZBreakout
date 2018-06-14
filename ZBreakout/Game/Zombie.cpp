#include "Zombie.h"
/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

Zombie::Zombie(ZombieType _type, float _attackSpeed, float _moveSpeed, int _maxhp) {
	this->type = _type;
	this->attackSpeed = _attackSpeed;
	this->moveSpeed = _moveSpeed;
	this->maxhp = _maxhp;

	this->hp = this->maxhp;
	this->dirty = false;
	this->ticksAlive = 0;
}

ZombieType Zombie::getType() {
	return type;
}

int Zombie::getMaxHp() {
	return maxhp;
}

float Zombie::getAttackSpeed() {
	return attackSpeed;
}

float Zombie::getMoveSpeed() {
	return moveSpeed;
}

bool Zombie::isAlive() {
	return hp >= 0;
}

void Zombie::addEffect(EffectType type, float duration) {
	Effect e;
	e.type = type;
	e.duration = duration;
	effects.push_back(e);
}

Zombie Zombie::createZombie(ZombieType type) {
	if (type == ZombieType::NORMAL) {
		return Zombie(type, 0.5f, 15.0f, 100);
	}

	return Zombie(type, 0.1f, 0.1f, 1);
}
