/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/
//Project includes
#include "IntroState.h"

const int ANIM_TIME = 250;
const std::string TITLE = "Tavern Games";

IntroState::IntroState(StateManager* mgr) 
: GameState(mgr){
	title.setFont(manager->getAssets().getFont("main"));
	title.setString("");
	title.setFillColor(sf::Color::White);

	manager->getAssets().setVolume(25);
	
	shotCircle.setFillColor(sf::Color::White);
	shotCircle.setRadius(7);
}

void IntroState::init() {
	_LOG_.log("IntroState", "Ready.");

	anim = 0;
}

void IntroState::render(sf::RenderWindow& window) {
	for (int i = 0; i < points.size(); i++) {
		shotCircle.setPosition(points[i]);
		window.draw(shotCircle);
	}

	window.draw(title);
}

void IntroState::input(sf::Event ev) {
	if (ev.type == sf::Event::KeyReleased) {
		if (ev.key.code == sf::Keyboard::Escape) {
			manager->setState(GameStates::MENU);
		}
	}
}

void IntroState::update() {
	if (anim != -1 && clock.getElapsedTime().asMilliseconds() > ANIM_TIME) {
		clock.restart();
		anim++;
		title.setString(TITLE.substr(0, anim));
		title.setPosition(manager->getWindowSize().x / 2 - title.getGlobalBounds().width / 2, 
			manager->getWindowSize().y / 2 - title.getGlobalBounds().height / 2);

		manager->getAssets().playSound("intro_shot");

		float sx = 0 + (rand() % static_cast<int>(manager->getWindowSize().x - 0 + 1));
		float sy = 0 + (rand() % static_cast<int>(manager->getWindowSize().y - 0 + 1));

		points.push_back(sf::Vector2f(sx, sy));

		if (anim >= TITLE.length()) {
			anim = -1;
		}
	} else {
		if (clock.getElapsedTime().asMilliseconds() > 3000) {
			manager->getAssets().setVolume(100);
			clock.restart();

			manager->setState(GameStates::MENU);
		}
	}
}