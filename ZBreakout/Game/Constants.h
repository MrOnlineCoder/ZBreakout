/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#ifndef CONSTANTS_H
#define CONSTANTS_H

typedef unsigned int PlayerID;

namespace Constants {
	const static int GAME_TICKRATE = 24;

	const static int GAME_PORT = 2801;
	
	const static int MAX_PLAYERS = 3;
	
	const static std::string VERSION = "1.0.0";
	
	const static std::string LEVEL = "lab";

	
	const static float PLAYER_SPEED = 20.0f;
};

#endif