/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>

#include "../Logger.h"
#include "Door.h"

#include <tmxlite/Map.hpp>
#include <iostream>

const static std::string LVL_START_NAME = "level_start";
const static std::string LVL_WALL = "wall";
const static std::string LVL_SPAWNER = "spawner";
const static std::string LVL_DOOR = "door";

class Level {
public:
	Level();

	bool loadFromFile(std::string path);

	bool collide(sf::FloatRect r);

	const sf::Vector2f& getStartPosition();
	std::vector<sf::FloatRect>& getWalls();
	std::vector<sf::Vector2f>& getSpawners();
	std::vector<Door>& getDoors();

	tmx::Map& getTMXMap();
private:
	void parseObject(const tmx::Object& obj);

	void addDoorsAsWalls();

	sf::Vector2f startPos;

	tmx::Map map;

	std::vector<sf::FloatRect> walls;
	std::vector<sf::Vector2f> spawners;
	std::vector<Door> doors;
};

#endif
