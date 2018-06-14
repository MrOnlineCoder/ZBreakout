/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#ifndef RANDOM_H
#define RANDOM_H

#include <random>

class Random {
public:
	template<typename T> static T& randomVectorElement(std::vector<T>& v);
	static int randomInt(int min, int max);
	static void init();
};

#endif
