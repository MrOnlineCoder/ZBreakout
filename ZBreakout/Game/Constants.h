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

	const static unsigned short CLIENT_PORT = 49228;
	const static unsigned short SERVER_PORT = 49229;
	
	const static int MAX_PLAYERS = 3;
	
	const static std::string VERSION = "1.0.0";
	
	const static std::string LEVEL = "chateau";

	const int MAX_PLAYER_HP = 200;

	const int PLAYER_SLOTS = 5;

	const int BULLET_SPEED = 40;

	const static float PLAYER_SPEED = 20.0f;

	const static int LEVEL_SIZE = 4096;

	const static int ACTION_DISTANCE = 120;
};

#endif