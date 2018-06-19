/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/
//Project includes
#include "CreditsState.h"

const std::string CREDITS_DATA = "--- ZBreakout ---\n"
"\n"
"Programming: Nikita Kogut (MrOnlineCoder)\n"
"Idea & Design: Shonyara (Oleksandr Desolator)\n"
"Music: Kevin MacLeod (incompetech.com)"
"\n"
"\n"
"Thanks to:\n"
"Laurent Gomila and SFML team\n"
"therocode\nDbug\nRosme\nMario\n"
"eXpl0it3r\nfallahn\njamesking from SFML Discord\n"
"StephenLynx\nBlueCobold\nJonny from SFML IRC\n"
""
"\n\n\n"
"THANKS FOR PLAYING! ;)"
;

const float CREDITS_SPEED = 70.2f;

CreditsState::CreditsState(StateManager* mgr)
	: GameState(mgr) {
	
}

void CreditsState::init() {
	_LOG_.log("CreditsState", "Ready.");

	credits.setString(CREDITS_DATA);
	credits.setFillColor(sf::Color::White);
	credits.setFont(manager->getAssets().getFont("main"));
	credits.setPosition(25, manager->getWindowSize().y+1);
}

void CreditsState::render(sf::RenderWindow& window) {
	window.draw(credits);
}

void CreditsState::input(sf::Event ev) {
	if (ev.type == sf::Event::KeyReleased) {
		if (ev.key.code == sf::Keyboard::Escape) {
			manager->setState(GameStates::MENU);
		}
	}
}

void CreditsState::update() {
	credits.move(0, -CREDITS_SPEED * manager->getDelta());
}