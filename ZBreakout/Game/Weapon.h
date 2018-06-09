/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#ifndef WEAPON_H
#define WEAPON_H

#include <SFML/Graphics.hpp>

#include "../Logger.h"
#include "Constants.h"

typedef std::string WeaponType;

//Defines a single bullet in game world.
struct Bullet {
	sf::Vector2f pos;
	float angle; //in degrees
	int damage;
	PlayerID shooter; //who shot that bullet
	WeaponType type;
};

class Weapon {
public:
	Weapon(WeaponType _type, int _maxammo, float _reloadTime, int _damage, float _fireDelay);
	Weapon();

	int getMaxAmmo();

	const WeaponType& getType();

	int getDamage();

	int getAmmo();

	//fire delay should be named fire rate
	//'0' means weapon is semi-automatic, like pistol
	//other value means delay between each shot
	float getFireDelay();

	//same as fireDelay == 0
	bool isAuto();

	float getReloadTime();

	//Creates a weapon instance of given type
	static Weapon createWeapon(std::string type);

	//For playing sounds / loading textures / GUI
	static std::string getWeaponTexturePath(std::string type);
	static std::string getWeaponShotSoundPath(std::string type);
	static std::string getWeaponName(std::string type);

	//Types of the weapons
	const static std::string PISTOL;
	const static std::string SHOTGUN;
	const static std::string AK47;
	const static std::string SHOCK;
	const static std::string SHIELD;
	const static std::string MACHINEGUN;
	const static std::string REVOLVER;

	//current ammo
	int ammo;

	//for fire rate
	sf::Clock fireClock;

	sf::Clock reloadClock;
private:
	int maxammo;
	float reloadTime;
	float fireDelay;
	WeaponType type;
	int damage;
};

#endif
