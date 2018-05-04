/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#ifndef LOGGER_H
#define LOGGER_H

//STD includes
#include <iostream>
#include <fstream>

//Thirdparty includes
#include <SFML/System.hpp>

//Project includes


class Logger {
public:
	bool init();
	void log(std::string tag, std::string msg);
	void close();
private:
	sf::Clock clock;
	std::ofstream file;
};

extern Logger _LOG_;

#endif