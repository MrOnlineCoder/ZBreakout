/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#include "AssetManager.h"

AssetManager::AssetManager() {
	isRandom = false;
	volume = 100;

	music.setLoop(false);
}

void AssetManager::loadAll() {
	//fonts
	loadFontFromFile("main", "fonts/OpenSans-Regular.ttf");

	//music
	addMusic("menu", "music/Crypto.ogg");

	//le player
	loadTexture("player", "player/idle.png");

	//intro
	loadSound("intro_shot", "sounds/intro_shot.ogg");
	loadSound("lol", "sounds/sha.ogg");

	//weapons
	loadTexture("weapon_"+Weapon::PISTOL, "weapons/pistol.png");
	loadSound("weapon_"+Weapon::PISTOL+"_shot", "sounds/pistol_shot.ogg");
	loadSound("weapon_" + Weapon::REVOLVER + "_shot", "sounds/revolver_shot.ogg");

	loadSound("weapon_reload", "sounds/reload.ogg");
}

bool AssetManager::init(char* arg0) {
	return true;
}

void AssetManager::destroy() {
	std::map<std::string, sf::Font*>::iterator fit;

	_LOG_.log("AssetManager", "Destroying fonts...");
	for ( fit = fonts.begin(); fit != fonts.end(); fit++ )
	{
		delete fit->second;
	}

	_LOG_.log("AssetManager", "Destroying textures...");
	std::map<std::string, sf::Texture*>::iterator tit; //NO TITS

	for ( tit = textures.begin(); tit != textures.end(); tit++ )
	{
		delete tit->second;
	}

	_LOG_.log("AssetManager", "Destroying sounds...");
	std::map<std::string, sf::SoundBuffer*>::iterator sit;

	for ( sit = sounds.begin(); sit != sounds.end(); sit++ )
	{
		delete sit->second;
	}

	std::list<sf::Sound*>::iterator i = playing.begin();
	while (i != playing.end())
	{
		delete *i;
		playing.erase(i++); 
	}

	_LOG_.log("AssetManager", "Succesfully deleted allocated resources");
}

void AssetManager::loadFontFromFile(std::string name, std::string path) {
	sf::Font* f = new sf::Font();

	if (!f->loadFromFile(ASSETS_PATH+"/"+path)) {
		_LOG_.log("AssetManager", "ERROR: Failed to load font from file "+path);
		return;
	}

	fonts.insert(std::make_pair(name, f));
	_LOG_.log("AssetManager", "Loaded font from file `"+name+"`");
}

void AssetManager::loadTexture(std::string name, std::string path) {
	sf::Texture* t = new sf::Texture();

	if (!t->loadFromFile(ASSETS_PATH + "/" + path)) {
		_LOG_.log("AssetManager", "ERROR: Failed to load texture "+path);
		return;
	}

	textures.insert(std::make_pair(name, t));
	_LOG_.log("AssetManager", "Loaded texture `"+name+"`");
}

void AssetManager::loadSound(std::string name, std::string path) {
	sf::SoundBuffer* t = new sf::SoundBuffer();

	if (!t->loadFromFile(ASSETS_PATH + "/" + path)) {
		_LOG_.log("AssetManager", "ERROR: Failed to load sound buffer "+path);
		return;
	}

	sounds.insert(std::make_pair(name, t));
	_LOG_.log("AssetManager", "Loaded sound "+name);
}

void AssetManager::playSound(std::string name) {
	sf::Sound* s = new sf::Sound();
	s->setBuffer(*sounds.at(name));
	s->setLoop(false);
	s->setVolume(volume);
	
	playing.push_back(s);

	s->play();
}

void AssetManager::update() {
	std::list<sf::Sound*>::iterator i = playing.begin();
	while (i != playing.end())
	{
		bool active = (*i)->getStatus() == sf::Sound::Playing;
		if (!(active))
		{
			(*i)->stop();
			delete *i;
			playing.erase(i++); 
		}
		else
		{
			++i;
		}
	}
}

void AssetManager::setVolume(float arg) {
	volume = arg;
	if (music.getStatus() == sf::Music::Status::Playing) music.setVolume(volume);
}

sf::Texture& AssetManager::getTexture(std::string key) {
	return *textures.at(key);
}

sf::Font& AssetManager::getFont(std::string name) {
	return *fonts.at(name);
}

void AssetManager::addMusic(std::string name, std::string path) {
	musics.insert(std::make_pair(name, path));
	_LOG_.log("AssetManager", "Added music "+name);
}

void AssetManager::playMusic(std::string name) {

	std::string path = musics.at(name);

	if (music.getStatus() == sf::SoundSource::Status::Playing) music.stop();
	
	if (!music.openFromFile(ASSETS_PATH+"/"+path)) {
		_LOG_.log("AssetManager", "ERROR: cannot start playing music from file"+name);
		return;
	}

	music.play();
	music.setVolume(volume);
}

void AssetManager::playRandomMusic(bool arg) {
	isRandom = arg;
}

void AssetManager::stopMusic() {
	music.stop();
	isRandom = false;
}