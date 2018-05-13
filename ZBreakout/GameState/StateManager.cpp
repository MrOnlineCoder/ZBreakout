/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#include "StateManager.h"

#include "IntroState.h"
#include "MenuState.h"
#include "CreditsState.h"
#include "PlayState.h"

void StateManager::init(sf::RenderWindow * window, AssetManager * assetsPtr, GameServer* serverPtr) {
	current = GameStates::INTRO;
	
	winSize = window->getSize();

	this->window = window;

	this->server = serverPtr;

	_LOG_.log("StateManager", "Loading AssetManager...");
	assets = assetsPtr;

	_LOG_.log("StateManager", "Creating IntroState");
	IntroState* introState = new IntroState(this);

	states.push_back(introState);

	_LOG_.log("StateManager", "Creating MenuState");
	MenuState* menuState = new MenuState(this);

	states.push_back(menuState);

	_LOG_.log("StateManager", "Creating CreditsState");
	CreditsState* creditsState = new CreditsState(this);

	states.push_back(creditsState);

	_LOG_.log("StateManager", "Creating PlayState");
	PlayState* playState = new PlayState(this);

	states.push_back(playState);

	_LOG_.log("StateManager", "Game states have been created");

	setState(GameStates::INTRO);
}


void StateManager::handleInput(sf::Event ev) {
	states[static_cast<int>(current)]->input(ev);
}

void StateManager::render() {
	states[static_cast<int>(current)]->render(*window);

	delta = frameClock.restart().asSeconds();
}
	
void StateManager::update() {
	states[static_cast<int>(current)]->update();
}

void StateManager::setState(GameStates newState) {
	current = newState;
	states[static_cast<int>(current)]->init();
}

float StateManager::getDelta() {
	return delta;
}

AssetManager & StateManager::getAssets(){
	return *assets;
}

sf::Vector2u StateManager::getWindowSize() {
	return winSize;
}

sf::RenderWindow & StateManager::getWindow() {
	return *window;
}

GameServer & StateManager::getServer() {
	return *server;
}
