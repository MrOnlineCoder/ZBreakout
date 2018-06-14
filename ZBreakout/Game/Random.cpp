/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/
#include "Random.h"

std::random_device rd;
std::mt19937 rng;

template<typename T> T& Random::randomVectorElement(std::vector<T>& v) {
	int index = Random::randomInt(0, v.size());
	return v[index];
}

int Random::randomInt(int min, int max) {
	std::uniform_int_distribution<int> uni(min, max);
	return uni(rng);
}

void Random::init() {
	rng.seed(rd());
}
