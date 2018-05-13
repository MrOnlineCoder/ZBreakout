/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

//STD includes

//Thirdparty includes
#include <SFML/Graphics.hpp>

//Project includes
#include "../Logger.h"
#include "GameState.h"
#include "StateManager.h"
#include "../GUI/TextButton.h"
#include "../Game/Game.h"
#include "../Game/Renderer.h"
#include "../Game/Chat.h"

class PlayState : public GameState {
public:
	PlayState(StateManager* mgr);

	void init();
	void render(sf::RenderWindow& window);
	void input(sf::Event ev);
	void update();


private:
	Game game;
	Renderer renderer;

	Chat chat;

	sf::View playerView;
	sf::View levelView;

	sf::CircleShape cc;

	void processNetwork();
	void processPacket(sf::Packet& packet);

	sf::Text status;
	bool loaded;

	int playerID;

	sf::TcpSocket socket;
};