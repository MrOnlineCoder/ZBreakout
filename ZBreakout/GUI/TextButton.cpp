/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#include "TextButton.h"

TextButton::TextButton() {

}

void TextButton::setString(sf::String str) {
	text.setString(str);
}

void TextButton::setColor(sf::Color c) {
	text.setFillColor(c);
	this->c = c;
}

void TextButton::setFont(sf::Font & fnt) {
	text.setFont(fnt);
}

void TextButton::setPosition(sf::Vector2f pos) {
	text.setPosition(pos);
}

void TextButton::setSize(int sz) {
	text.setCharacterSize(sz);
}

bool TextButton::process(sf::Event ev) {
	if (ev.type == sf::Event::MouseMoved) {
		bool flag = text.getGlobalBounds().contains(sf::Vector2f(ev.mouseMove.x, ev.mouseMove.y));
		text.setFillColor(flag ? sf::Color::Yellow : c);
		return false;
	}

	if (ev.type == sf::Event::MouseButtonReleased) {
		bool flag = text.getGlobalBounds().contains(sf::Vector2f(ev.mouseButton.x, ev.mouseButton.y));
		return flag;
	}

	return false;
}

void TextButton::render(sf::RenderWindow & wnd) {
	wnd.draw(text);
}
