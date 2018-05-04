/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#ifndef STATEMGR_H
#define STATEMGR_H
//STD includes
#include <vector>

//Thirdparty includes
#include <SFML/Graphics.hpp>

//Project includes
#include "../Logger.h"
#include "../AssetManager.h"
#include "../Game/GameServer.h"

class GameState;

enum class GameStates {
	INTRO = 0,
	MENU = 1,
	CREDITS = 2,
	GAME = 3
};

class StateManager {
public:
	void init(sf::RenderWindow* window, AssetManager* assetsPtr, GameServer* serverPtr);

	void handleInput(sf::Event ev);
	void render();
	void update();

	void setState(GameStates newState);

	float getDelta();

	AssetManager& getAssets();
	sf::Vector2u getWindowSize();
	sf::RenderWindow& getWindow();

	GameServer& getServer();
private:
	std::vector<GameState*> states;
	GameStates current;
	sf::Vector2u winSize;
	sf::RenderWindow* window;

	GameServer* server;
	AssetManager* assets;

	float delta;
	sf::Clock frameClock;
};
#endif