/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#include "Logger.h"

Logger _LOG_;

bool Logger::init() {
	file.open("game.log");

	if (!file) return false;

	clock.restart();

	return true;
}

void Logger::log(std::string tag, std::string msg) {
	file << "[" << tag << " at " << clock.getElapsedTime().asSeconds() << " s.] " << msg << "\n";
}

void Logger::close() {
	file.close();
}