/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics.hpp>

#include "Player.h"

#include "../Logger.h"

#include "../GameState/StateManager.h"

#include "SFMLOrthogonalLayer.hpp"

#include "RichText.h"

#include "../GUI/RoundedShape.hpp"

const int HPBAR_SIZE = 250;
const int HPBAR_HEIGHT = 20;
const float HPBAR_OUTLINE = 3.0f;

const int ITEMTEXT_PADDING = 100;

const float BULLET_RADIUS = 2.5f;

const int ZOMBIEBAR_SIZE = 100;

struct RenderDebugData {
	std::vector<sf::FloatRect>* walls;
	bool enabled;
};

struct DamageHit {
	int dmg;
	sf::Vector2f pos;
	float scale;
	sf::Clock alive;
};

class Renderer {
	public:
		Renderer();

		void init(StateManager& mgr);

		void setLevel(tmx::Map& map);

		void drawPlayer(Player& pl);
		void drawPlayerHPBar(Player& pl);
		void drawInventory(Player& pl);
		void drawBullet(Bullet& b);
		void drawLevel();
		void drawDebug();
		void drawZombie(Zombie& z);

		void updateItemText(Player& pl);
		
		void playDamageEffect(sf::Vector2f pos, int dmg);

		void renderDamageHits();

		sf::Vector2f getPlayerCenter(sf::Vector2f arg);

		RenderDebugData debug;
	private:
		void setup();

		sf::RenderWindow* window;
		StateManager* manager;
		
		//Level
		MapLayer levelTiles;

		//Player
		sf::Text plName;
		sf::Sprite plSpr;

		//Hp bar
		sf::RectangleShape hpBar;
		sf::RectangleShape hpBarOut;

		//Items and inventory
		sfe::RichText wpnText; //weapon title

		//gold
		sf::RoundedRectangleShape goldShape;
		sfe::RichText goldText;

		//Bullets
		sf::CircleShape bulletShape;
		std::vector<DamageHit> hits;
		sf::Text hitTxt;

		//Zombies
		sf::Sprite zombieSpr;
		sf::RectangleShape zombieHp;

		//debug
		sf::RectangleShape wallRect;
		sf::RectangleShape plRect;
};

#endif