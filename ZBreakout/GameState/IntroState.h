/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

//STD includes

//Thirdparty includes
#include <SFML/Graphics.hpp>

//Project includes
#include "../Logger.h"
#include "GameState.h"
#include "StateManager.h"


class IntroState : public GameState {
public:
	IntroState(StateManager* mgr);

	void init();
	void render(sf::RenderWindow& window);
	void input(sf::Event ev);
	void update();

private:
	sf::Text title;
	sf::Clock clock;

	sf::CircleShape shotCircle;

	std::vector<sf::Vector2f> points;

	int anim;
};