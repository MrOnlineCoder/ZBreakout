/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#include "Renderer.h"

void Renderer::init(StateManager & mgr){
	manager = &mgr;
	window = &manager->getWindow();

	_LOG_.log("Renderer", "Initializing Renderer...");

	setup();

	_LOG_.log("Renderer", "Renderer ready.");
}

void Renderer::drawPlayer(Player & pl) {

	plSpr.setPosition(pl.pos);
	plSpr.setRotation(pl.direction * 90);
	plSpr.setOrigin(plSpr.getGlobalBounds().width / 2, plSpr.getGlobalBounds().height / 2);

	window->draw(plSpr);

	plName.setString(pl.getNickname());

	sf::Vector2f namePos = plSpr.getPosition();

	namePos.x = plSpr.getPosition().x - plName.getGlobalBounds().width / 2;
	namePos.y = plSpr.getPosition().y - (plName.getGlobalBounds().height * 6) - 5;

	plName.setPosition(namePos);

	window->draw(plName);
}

sf::Vector2f Renderer::getPlayerCenter(sf::Vector2f arg) {
	sf::Vector2f res;

	res.x = arg.x;
	res.y = arg.y;

	return res;
}

void Renderer::setup() {
	plSpr.setTexture(manager->getAssets().getTexture("player"));

	plName.setFillColor(sf::Color::White);
	plName.setCharacterSize(18);
	plName.setFont(manager->getAssets().getFont("main"));
}
