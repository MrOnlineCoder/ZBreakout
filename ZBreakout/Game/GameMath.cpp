#include "GameMath.h"

#include <cmath>

float GameMath::distance(sf::Vector2f& a, sf::Vector2f& b) {
	return std::sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2));
}

float GameMath::distance(sf::Vector2i& a, sf::Vector2i& b) {
	return std::sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2));
}

float GameMath::distanceSquared(sf::Vector2f & a, sf::Vector2f & b) {
	return std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2);
}

sf::Vector2f GameMath::interpolate(const sf::Vector2f & pointA, const sf::Vector2f & pointB, float factor)
{
	if (factor > 1.f)
		factor = 1.f;

	else if (factor < 0.f)
		factor = 0.f;

	return pointA + (pointB - pointA) * factor;
}

sf::Vector2f GameMath::findVelocity(sf::Vector2f & start, sf::Vector2f & end, float speed) {
	float d = distance(start, end);
	float ratio = speed / d;

	return sf::Vector2f(ratio * (end.x - start.x), ratio * (end.y - start.y));
}
