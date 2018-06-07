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

	tilesBg.loadFromMap(map, 0);
	objects.loadFromMap(map, 1);

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

	debugRect.setFillColor(sf::Color(255, 255, 255, 5));
	debugRect.setOutlineColor(sf::Color::White);
	debugRect.setOutlineThickness(2.0f);

	return true;
}

void Level::render(sf::RenderWindow & window) {
	window.draw(tilesBg);

	if (debugMode) {
		for (auto& rect : solids) {
			debugRect.setSize(sf::Vector2f(rect.width, rect.height));
			debugRect.setPosition(sf::Vector2f(rect.left, rect.top));

			window.draw(debugRect);
		}
	}
}

bool Level::collide(sf::FloatRect r)
{
	for (auto& rect : solids) {
		if (rect.intersects(r)) {
			return true;
		}
	}

	return false;
}

const sf::Vector2f & Level::getStartPosition() {
	return startPos;
}

void Level::setDebugMode(bool arg) {
	debugMode = arg;
}

void Level::parseObject(const tmx::Object & obj) {
	if (obj.getName() == LVL_START_NAME) {
		startPos = sf::Vector2f(obj.getPosition().x, obj.getPosition().y);
		std::cout << "Parsed start pos " << startPos.x << ", " << startPos.y << std::endl;
		return;
	}

	if (obj.getType() == LVL_SOLID) {
		sf::FloatRect rect;
		rect.left = obj.getAABB().left;
		rect.top = obj.getAABB().top;
		rect.width = obj.getAABB().width;
		rect.height = obj.getAABB().height;

		solids.push_back(rect);
		std::cout << "Parsed solid object: " << rect.left << "x, " << rect.top << "y, " << rect.width << "w, " << rect.height << "y \n";
		return;
	}
}
