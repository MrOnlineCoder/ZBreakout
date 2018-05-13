/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#include "Chat.h"

Chat::Chat() {

}

void Chat::init(AssetManager& asst, int x, int y) {
	log.setFont(asst.getFont("main"));
	log.setCharacterSize(20);
	log.setPosition(sf::Vector2f(x, y - 50 - 5));

	this->x = x;
	this->y = y;
}

void Chat::render(sf::RenderWindow& window) {
	window.draw(log);
}

void Chat::addMessage(sf::Color c, std::string message) {
	ChatMessage msg;
	msg.color = c;
	msg.content = message;

	messages.push_back(msg);

	if (messages.size() <= CHAT_MAX_MESSAGES) {
		log.clear();
		for (int i = 0; i<messages.size(); i++) {
			log << messages[i].color << messages[i].content << "\n";
		}

		log.setPosition(x, y - messages.size()*log.getCharacterSize() - 20);
	}
	else {
		log.clear();
		for (int i = messages.size() - CHAT_MAX_MESSAGES; i<messages.size(); i++) {
			log << messages[i].color << messages[i].content << "\n";
		}
	}
}