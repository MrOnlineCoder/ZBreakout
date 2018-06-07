/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#ifndef ASSETMGR_H
#define ASSETMGR_H

//STD includes
#include <map>
#include <string>
#include <list>

//Thirdparty includes
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

//Project includes
#include "Logger.h"
#include "Game/Weapon.h"

const std::string ASSETS_PATH = "Assets";

class AssetManager {
	public:
		AssetManager();
		void loadAll();
		bool init(char* arg0);

		void destroy();

		void loadFont(std::string name, std::string path);
		void loadFontFromFile(std::string name, std::string path);
		void loadTexture(std::string name, std::string path);
		void loadSound(std::string name, std::string path);
		
		void playSound(std::string name);

		void playMusic(std::string name);
		void addMusic(std::string name, std::string path);
		void playRandomMusic(bool arg);

		void stopMusic();

		void update();

		void setVolume(float arg);

		sf::Texture& getTexture(std::string key);
		sf::Font& getFont(std::string name);
	private:
		std::map<std::string, sf::Texture*> textures;
		std::map<std::string, sf::Font*> fonts;
		std::map<std::string, sf::SoundBuffer*> sounds;
		std::map<std::string, std::string> musics;
		std::list<sf::Sound*> playing;

		bool isRandom;

		float volume;

		sf::Music music;
};

#endif