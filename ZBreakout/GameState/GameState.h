/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#ifndef GAMESTATE_H
#define GAMESTATE_H
//STD includes


//Thirdparty includes
#include <SFML/Graphics.hpp>

//Project includes
class StateManager;

class GameState {
public:
	GameState(StateManager* mgr) {
		manager = mgr;
	}

	virtual void init() = 0;
	virtual void render(sf::RenderWindow& window) = 0;
	virtual void input(sf::Event ev) = 0;
	virtual void update() = 0;
protected:
	StateManager* manager;
};
#endif