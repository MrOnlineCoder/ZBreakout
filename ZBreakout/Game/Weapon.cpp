#include "Weapon.h"

//set weapon types
const std::string Weapon::PISTOL = "pistol";
const std::string Weapon::AK47 = "ak47";
const std::string Weapon::SHOTGUN = "shotgun";
const std::string Weapon::MACHINEGUN = "machinegun";
const std::string Weapon::REVOLVER = "revolver";
const std::string Weapon::SHOCK = "shockgun";
const std::string Weapon::SHIELD = "shield";

Weapon::Weapon(WeaponType _type, int _maxammo, float _reloadTime, int _damage, float _fireDelay) {
	this->type = _type;
	this->maxammo = _maxammo;
	this->reloadTime = _reloadTime;
	this->ammo = this->maxammo;
	this->damage = _damage;
	this->fireDelay = _fireDelay;
}

Weapon::Weapon() {
	this->type = PISTOL;
	this->maxammo = 0;
	this->reloadTime = 0;
	this->ammo = this->maxammo;
	this->damage = 0;
}

int Weapon::getMaxAmmo() {
	return maxammo;
}

const WeaponType & Weapon::getType() {
	return type;
}

int Weapon::getDamage() {
	return damage;
}

int Weapon::getAmmo() {
	return ammo;
}

float Weapon::getFireDelay() {
	return fireDelay;
}

bool Weapon::isAuto() {
	return fireDelay != 0.0f;
}

float Weapon::getReloadTime() {
	return reloadTime;
}

//Weapon instances Factory
//Change to change gameplay :)
//
//Pistol definition is REQUIRED, infinite recursion will occur if missing
Weapon Weapon::createWeapon(std::string type) {
	if (type == Weapon::PISTOL) {
		return Weapon(Weapon::PISTOL, 20, 1.5f, 3, 0.0f);
	}

	if (type == Weapon::REVOLVER) {
		return Weapon(Weapon::REVOLVER, 8, 1.5f, 3, 0.0f);
	}

	if (type == Weapon::AK47) {
		return Weapon(Weapon::AK47, 30, 2.5f, 5, 0.1f);
	}

	return Weapon::createWeapon(Weapon::PISTOL);
}

std::string Weapon::getWeaponTexturePath(std::string type) {
	return "weapon_" + type;
}

std::string Weapon::getWeaponShotSoundPath(std::string type)
{
	return "weapon_" + type + "_shot";
}

std::string Weapon::getWeaponName(std::string type) {
	if (type == Weapon::PISTOL) {
		return "Pistol";
	}

	if (type == Weapon::AK47) {
		return "AK-47";
	}

	if (type == Weapon::REVOLVER) {
		return "Revolver";
	}

	if (type == Weapon::SHOTGUN) {
		return "Shotgun";
	}
	
	if (type == Weapon::SHIELD) {
		return "Shield Gun";
	}

	if (type == Weapon::SHOCK) {
		return "Shock Rifle";
	}

	return "???";
}


