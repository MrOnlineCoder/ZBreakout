/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/
#ifndef CHAT_H
#define CHAT_H

//STD includes
#include <string>

//Thirdparty includes
#include <SFML/Graphics.hpp>

//Project includes
#include "../AssetManager.h"
#include "RichText.h"

const int CHAT_MAX_MESSAGES = 5;

struct ChatMessage {
	std::string content;
	sf::Color color;
};

class Chat {
public:
	Chat();

	void init(AssetManager& asst, int x, int y);

	void render(sf::RenderWindow& window);

	void addMessage(sf::Color c, std::string message);
private:
	sfe::RichText log;
	std::vector<ChatMessage> messages;

	int x;
	int y;
};

#endif