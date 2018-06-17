#include "Pathsearch.h"
#include "../Logger.h"
#include "GameMath.h"

static std::map<sf::Vector2i, bool, cmpByVector2i> closed;

static std::vector<sf::Vector2i> directions;

static sf::Vector2i BLOCKED_NODE = sf::Vector2i(-1,-1);

int estimateCost(PathData& path, sf::Vector2i pos) {
	return GameMath::distance(pos, path.end) + GameMath::distance(path.current, path.start);
}

void Pathsearch::initPath(PathData* path, sf::Vector2f start, sf::Vector2f end) {
	path->start = sf::Vector2i(start.x / PATH_GRID_TILE_SIZE, start.y / PATH_GRID_TILE_SIZE);
	path->end = sf::Vector2i(end.x / PATH_GRID_TILE_SIZE, end.y / PATH_GRID_TILE_SIZE);
	path->current = path->start;
	path->closed.clear();
}

sf::Vector2i Pathsearch::searchNextNode(PathData & path) {
	if (path.current == path.end) {
		return path.current;
	}

	sf::Vector2i min = BLOCKED_NODE;
	int minCost = PATH_GRID_TILE_SIZE * PATH_GRID_SIZE;

	for (int d = 0; d < directions.size(); d++) {
		sf::Vector2i neigh = path.current + directions[d];

		if (closed.count(neigh) || path.closed.count(neigh)) {
			continue;
		}

		int _min = estimateCost(path, neigh);

		if (directions[d].x != 0 && directions[d].y != 0) _min *= 1.4;

		if (_min < minCost) {
			minCost = _min;
			min = neigh;
		}
	}

	if (min != BLOCKED_NODE) {
		path.closed[path.current] = true;
		path.current = min;
	}

	return min;
}

sf::Vector2f Pathsearch::mapNode(sf::Vector2i node) {
	return sf::Vector2f(node.x * PATH_GRID_TILE_SIZE, node.y * PATH_GRID_TILE_SIZE);
}

void Pathsearch::rasterizeGrid(std::vector<sf::FloatRect>& walls, sf::Vector2f enemySize) {
	_LOG_.log("Pathsearch", "Rasterizing level containing "+std::to_string(walls.size())+" walls.");

	int padx = enemySize.x / PATH_GRID_TILE_SIZE;
	int pady = enemySize.y / PATH_GRID_TILE_SIZE;

	for (int i = 0; i < walls.size(); i++) {
		sf::FloatRect& rect = walls.at(i);

		int gx = (int) rect.left / PATH_GRID_TILE_SIZE;
		int gy = (int) rect.top / PATH_GRID_TILE_SIZE;
		int gw = (int) rect.width / PATH_GRID_TILE_SIZE;
		int gh = (int) rect.height / PATH_GRID_TILE_SIZE;

		for (int ay = -pady/2; ay < gh+pady/2; ay++) {
			for (int ax = -padx/2; ax < gw+padx/2; ax++) {
				sf::Vector2i vec = sf::Vector2i(gx + ax, gy + ay);
				closed[vec] = true;
			}
		}
	}

	directions.push_back(sf::Vector2i(-1, -1)); //top-left
	directions.push_back(sf::Vector2i(0, -1)); //top-middle
	directions.push_back(sf::Vector2i(1, -1)); //top-right
	directions.push_back(sf::Vector2i(-1, 0)); //middle-left
	directions.push_back(sf::Vector2i(1, 0)); //middle-right
	directions.push_back(sf::Vector2i(-1, 1)); //bottom-left
	directions.push_back(sf::Vector2i(0, 1)); //bottom-middle
	directions.push_back(sf::Vector2i(1, 1)); //bottom-right

	_LOG_.log("Pathsearch","Razterization complete, marked as closed "+std::to_string(closed.size())+" cells.");
}
