/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <SFML/Graphics.hpp>

#include "../Logger.h"
#include "Constants.h"
#include "Effect.h"

enum class ZombieType {
	NORMAL
};

class Zombie{
public:
	Zombie(ZombieType _type, float _attackSpeed, float _moveSpeed, int _maxhp);
	
	ZombieType getType();

	sf::Vector2f pos;
	sf::Vector2f velocity;
	PlayerID target;
	int hp;

	bool dirty;
	int ticksAlive;

	int getMaxHp();
	float getAttackSpeed();
	float getMoveSpeed();
	bool isAlive();

	void addEffect(EffectType type, float duration);

	std::vector<Effect> effects;

	static Zombie createZombie(ZombieType type);
private:
	ZombieType type;
	int damage;
	float attackSpeed;
	int maxhp;
	float moveSpeed;
};

#endif
