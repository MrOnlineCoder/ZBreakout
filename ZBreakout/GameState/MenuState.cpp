/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/
//Project includes
#include "MenuState.h"

MenuState::MenuState(StateManager* mgr)
	: GameState(mgr) {
	playBtn.setFont(manager->getAssets().getFont("main"));
	playBtn.setString("Create game");
	playBtn.setPosition(sf::Vector2f(50, 50));
	playBtn.setColor(sf::Color::White);

	connectBtn.setFont(manager->getAssets().getFont("main"));
	connectBtn.setString("Join a game");
	connectBtn.setPosition(sf::Vector2f(50, 100));
	connectBtn.setColor(sf::Color::White);

	exitBtn.setFont(manager->getAssets().getFont("main"));
	exitBtn.setString("Quit");
	exitBtn.setPosition(sf::Vector2f(50, 350));
	exitBtn.setColor(sf::Color::White);

	creditsBtn.setFont(manager->getAssets().getFont("main"));
	creditsBtn.setString("Credits");
	creditsBtn.setPosition(sf::Vector2f(50, 200));
	creditsBtn.setColor(sf::Color::White);
}

void MenuState::init() {
	_LOG_.log("MenuState", "Ready.");

	manager->getAssets().playMusic("menu");
}

void MenuState::render(sf::RenderWindow& window) {
	playBtn.render(window);
	exitBtn.render(window);
	creditsBtn.render(window);
	connectBtn.render(window);
}

void MenuState::input(sf::Event ev) {
	if (exitBtn.process(ev)) {
		manager->getWindow().close();
		return;
	}

	if (playBtn.process(ev)) {
		manager->getWindow().close();
		return;
	}

	if (connectBtn.process(ev)) {
		manager->getWindow().close();
		return;
	}

	if (creditsBtn.process(ev)) {
		manager->setState(GameStates::CREDITS);
		return;
	}
}

void MenuState::update() {
	
}