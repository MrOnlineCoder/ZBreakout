/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#include "Level.h"

Level::Level() {
	spawners.reserve(32);
	walls.reserve(256);
	doors.reserve(16);
}

bool Level::loadFromFile(std::string path) {
	if (!map.load(path)) {
		_LOG_.log("Level", "Failed to find/load file "+path);
		return false;
	}

	const auto& layers = map.getLayers();

	for (const auto& layer : layers) {
		if (layer->getType() == tmx::Layer::Type::Object)
		{
			const auto& objects = dynamic_cast<tmx::ObjectGroup*>(layer.get())->getObjects();

			for (std::size_t o = 0; o < objects.size(); o++) {
				parseObject(objects[o]);
			}
		}
	}

	addDoorsAsWalls();

	_LOG_.log("Level", "Loaded level "+path);
	return true;
}

bool Level::collide(sf::FloatRect r)
{
	for (auto& rect : walls) {
		if (rect.intersects(r)) {
			return true;
		}
	}

	return false;
}

const sf::Vector2f & Level::getStartPosition() {
	return startPos;
}

std::vector<sf::FloatRect>& Level::getWalls() {
	return walls;
}

std::vector<sf::Vector2f>& Level::getSpawners() {
	return spawners;
}

std::vector<Door>& Level::getDoors() {
	return doors;
}

tmx::Map & Level::getTMXMap() {
	return map;
}

void Level::parseObject(const tmx::Object & obj) {
	if (obj.getName() == LVL_START_NAME) {
		startPos = sf::Vector2f(obj.getPosition().x, obj.getPosition().y);
		_LOG_.log("Level", "Found object StartPosition: "+std::to_string(obj.getPosition().x)+", "+std::to_string(obj.getPosition().y));
		return;
	}

	if (obj.getType() == LVL_WALL) {
		sf::FloatRect rect;
		rect.left = obj.getAABB().left;
		rect.top = obj.getAABB().top;
		rect.width = obj.getAABB().width;
		rect.height = obj.getAABB().height;

		walls.push_back(rect);
		_LOG_.log("Level", "Found object Wall: " + std::to_string(obj.getPosition().x) + ", " + std::to_string(obj.getPosition().y) + ", w="+std::to_string(obj.getAABB().width)+", h="+ std::to_string(obj.getAABB().width));
		return;
	}

	if (obj.getType() == LVL_SPAWNER) {
		sf::Vector2f pos;

		pos.x = obj.getPosition().x;
		pos.y = obj.getPosition().y;

		spawners.push_back(pos);
		_LOG_.log("Level", "Found object Spawner: " + std::to_string(pos.x) + ", " + std::to_string(pos.y));
		return;
	}

	if (obj.getType() == LVL_DOOR) {
		sf::FloatRect rect;
		rect.left = obj.getAABB().left;
		rect.top = obj.getAABB().top;
		rect.width = obj.getAABB().width;
		rect.height = obj.getAABB().height;

		Door door;
		door.bounds = rect;
		door.name = obj.getName();
		door.price = obj.getProperties()[0].getIntValue();
		door.open = false;
		doors.push_back(door);

		_LOG_.log("Level", "Found object Door: " + std::to_string(door.bounds.left) + ", " + std::to_string(door.bounds.top) + ", name= "+obj.getName());
		return;
	}
}

void Level::addDoorsAsWalls() {
	for (auto i = 0; i < doors.size(); i++) {
		walls.push_back(doors[i].bounds);
		doors[i].index = walls.size() - 1;
	}

	_LOG_.log("Level", "Added additional "+std::to_string(doors.size())+" doors to solids list.");
}
