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

}

bool Level::loadFromFile(std::string path) {
	if (!map.load(path)) {
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

tmx::Map & Level::getTMXMap() {
	return map;
}

void Level::parseObject(const tmx::Object & obj) {
	if (obj.getName() == LVL_START_NAME) {
		startPos = sf::Vector2f(obj.getPosition().x, obj.getPosition().y);
		std::cout << "Parsed start pos " << startPos.x << ", " << startPos.y << std::endl;
		return;
	}

	if (obj.getType() == LVL_WALL) {
		sf::FloatRect rect;
		rect.left = obj.getAABB().left;
		rect.top = obj.getAABB().top;
		rect.width = obj.getAABB().width;
		rect.height = obj.getAABB().height;

		walls.push_back(rect);
		std::cout << "Parsed wall object: " << rect.left << "x, " << rect.top << "y, " << rect.width << "w, " << rect.height << "y \n";
		return;
	}
}
