/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#ifndef PATHSEARCH_H
#define PATHSEARCH_H

#include <SFML/Graphics.hpp>
#include <map>
#include <vector>

const int PATH_GRID_SIZE = 256;
const int PATH_GRID_TILE_SIZE = 16;

struct cmpByVector2i {
	bool operator()(const sf::Vector2i& a, const sf::Vector2i& b) const {
		return std::make_pair(a.x, a.y) < std::make_pair(b.x, b.y);
	}
};

struct PathNeighbour {
	sf::Vector2i pos;
	int cost;
};

struct PathData { 
	sf::Vector2i start;
	sf::Vector2i end;
	sf::Vector2i current;
	std::map<sf::Vector2i, bool, cmpByVector2i> closed;
};

namespace Pathsearch {
	void initPath(PathData* path, sf::Vector2f start, sf::Vector2f end);
	sf::Vector2i searchNextNode(PathData& path);
	sf::Vector2f mapNode(sf::Vector2i node);
	void rasterizeGrid(std::vector<sf::FloatRect>& walls, sf::Vector2f enemySize);
}

#endif