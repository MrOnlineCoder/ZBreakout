#include "GameMath.h"

#include <cmath>

float GameMath::distance(sf::Vector2f a, sf::Vector2f b) {
	return std::sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2));
}

float GameMath::distance(sf::Vector2i a, sf::Vector2i b) {
	return std::sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2));
}
