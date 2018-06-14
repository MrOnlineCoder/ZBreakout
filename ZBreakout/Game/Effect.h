/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#ifndef EFFECT_H
#define EFFECT_H

#include <SFML/Network.hpp>

#include "../Logger.h"

#include "Constants.h"

enum class EffectType {
	BURNING,
	BLEEDING,
	DECAY,
	STUNNED,
	SLOWNESS
};

struct Effect {
	EffectType type;
	float duration;
	sf::Clock clock;
};

#endif
