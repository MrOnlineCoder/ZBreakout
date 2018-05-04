/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#ifndef ENGINE_H
#define ENGINE_H

//STD includes

//Thirdparty includes
#include <SFML/Graphics.hpp>

//Project includes
#include "Logger.h"
#include "GameState/StateManager.h"
#include "AssetManager.h"

#define ELOG(x) _LOG_.log("Engine", x);

class Engine {
public:
	Engine(int argc, char* argv[]);

	int run();
private:
	void shutdown();

	sf::RenderWindow window;
	StateManager stateManager;

	AssetManager assets;
};

#endif