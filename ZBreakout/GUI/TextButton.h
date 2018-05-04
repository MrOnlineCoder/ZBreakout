/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#ifndef TXTBUTTON_H
#define TXTBUTTON_H

#include <SFML/Graphics.hpp>

class TextButton {
public:
	TextButton();
	void setString(sf::String str);
	void setColor(sf::Color c);
	void setFont(sf::Font& fnt);
	void setPosition(sf::Vector2f pos);
	void setSize(int sz);
	bool process(sf::Event ev);
	void render(sf::RenderWindow& wnd);
private:
	sf::Text text;
	sf::Color c;
};

#endif
