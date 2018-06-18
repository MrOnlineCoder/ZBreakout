/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

//STD includes
#include <sstream>

//Thirdparty includes
#include <SFML/Graphics.hpp>

//Project includes
#include "GameState.h"
#include "StateManager.h"

#include "../Logger.h"

#include "../GUI/TextButton.h"
#include "../GUI/FPS.h"

#include "../Game/Game.h"
#include "../Game/Renderer.h"
#include "../Game/Chat.h"
#include "../Game/Level.h"

class PlayState : public GameState {
public:
	PlayState(StateManager* mgr);

	void init();
	void render(sf::RenderWindow& window);
	void input(sf::Event ev);
	void update();

private:
	void loadThreadFunc();
	sf::Thread lvlLoadThread;

	Game game;
	Renderer renderer;

	Chat chat;

	Level lvl;
	std::string levelName;

	sf::View playerView;
	sf::View guiView;

	sf::CircleShape cc;

	void processPacket(sf::Packet& packet);

	void setPlayerCurrentSlot(int slot);
	void shoot();

	sf::Text status;
	bool loaded;

	int playerID;

	sf::UdpSocket socket;

	sf::Clock clock;

	sf::Clock soundClock;
	long zombieRoarDelay;

	sf::IpAddress serverIP;

	bool debug;
	sf::Text debugTxt;
	std::stringstream debugStream;
	FPS fps;

	unsigned short port;
	int packets;
	size_t netDataSize;
};