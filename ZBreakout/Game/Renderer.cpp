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

void Renderer::drawPlayerHPBar(Player & pl) {
	if (pl.hp < (Constants::MAX_PLAYER_HP * 0.20)) {
		hpBar.setFillColor(sf::Color::Red);
	} else {
		hpBar.setFillColor(sf::Color(113, 219, 94));
	}

	hpBar.setSize(sf::Vector2f((pl.hp * HPBAR_SIZE) / Constants::MAX_PLAYER_HP, HPBAR_HEIGHT));
	window->draw(hpBar);
	window->draw(hpBarOut);
}

void Renderer::drawInventory(Player & pl) {
	window->draw(wpnText);
}

void Renderer::drawBullet(Bullet& b) {
	bulletShape.setPosition(b.pos);
	window->draw(bulletShape);
}

void Renderer::updateItemText(Player& pl) {
	std::string wpnString = "";

	if (pl.isSlotFree(pl.currentSlot)) {
		wpnString = "[ Empty ]";
	} else if (pl.getItem().type == ItemType::WEAPON) {
		Weapon& wpn = pl.getWeapon();

		if (wpn.getAmmo() == 0) {
			wpnString = "[ " + Weapon::getWeaponName(wpn.getType()) + " ] Reloading...";
		} else {
			wpnString = "[ " + Weapon::getWeaponName(wpn.getType()) + " ] " + std::to_string(wpn.getAmmo()) + " / " + std::to_string(wpn.getMaxAmmo());
		}

		
	}

	wpnText.setString(wpnString);
	wpnText.setPosition(sf::Vector2f(manager->getWindowSize().x / 2 - wpnText.getGlobalBounds().width / 2,
		manager->getWindowSize().y - ITEMTEXT_PADDING - wpnText.getGlobalBounds().height));
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

	//hp bar
	hpBar.setSize(sf::Vector2f(HPBAR_SIZE, HPBAR_HEIGHT));
	hpBar.setPosition(manager->getWindowSize().x - HPBAR_SIZE - HPBAR_OUTLINE - 15, 15);

	hpBarOut.setOutlineColor(sf::Color::White);
	hpBarOut.setFillColor(sf::Color(255,255,255,0));
	hpBarOut.setPosition(hpBar.getPosition().x - HPBAR_OUTLINE, hpBar.getPosition().y - HPBAR_OUTLINE);
	hpBarOut.setOutlineThickness(HPBAR_OUTLINE);
	hpBarOut.setSize(sf::Vector2f(hpBar.getGlobalBounds().width + HPBAR_OUTLINE, 
		hpBar.getGlobalBounds().height + HPBAR_OUTLINE));
	
	//items and inventory
	wpnText.setFillColor(sf::Color::White);
	wpnText.setCharacterSize(24);
	wpnText.setFont(manager->getAssets().getFont("main"));

	//bullets
	bulletShape.setFillColor(sf::Color::Yellow);
	bulletShape.setRadius(BULLET_RADIUS);
}
