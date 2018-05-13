/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics.hpp>

#include "Player.h"

#include "../Logger.h"

#include "../GameState/StateManager.h"

class Renderer {
	public:
		void init(StateManager& mgr);
		void drawPlayer(Player& pl);

		sf::Vector2f getPlayerCenter(sf::Vector2f arg);
	private:
		void setup();

		sf::RenderWindow* window;
		StateManager* manager;

		//Player
		sf::Text plName;
		sf::Sprite plSpr;
};

#endif