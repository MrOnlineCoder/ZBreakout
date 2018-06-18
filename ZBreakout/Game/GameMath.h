/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#ifndef GAMEMATH_H
#define GAMEMATH_H

#include <SFML/System/Vector2.hpp>

namespace GameMath {
	float distance(sf::Vector2f& a, sf::Vector2f& b);
	float distance(sf::Vector2i& a, sf::Vector2i& b);
	float distanceSquared(sf::Vector2f& a, sf::Vector2f& b);

	sf::Vector2f interpolate(const sf::Vector2f& pointA, const sf::Vector2f& pointB, float factor);
	sf::Vector2f findVelocity(sf::Vector2f& start, sf::Vector2f& end, float speed);
};

#endif