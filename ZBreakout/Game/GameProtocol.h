/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#ifndef GAMEPROTOCOL_H
#define GAMEPROTOCOL_H

#include <SFML/Network.hpp>

#include "../Logger.h"
#include "Constants.h"
#include "Player.h"
#include "Zombie.h"

/*
	ZBreakout Network Game Protocol:

	Each field in this enum defines a Network Message
	It is casted to int when sent over network.

	============================================================
	!!! DO NOT CHANGE THE ORDER OF FIELDS AFTER GAME RELEASE !!!

	    !!! THIS CAN BREAK THE GAME FOR OLDER CLIENTS !!!
	============================================================

	SV_ prefix means 'sent by server to clients'
	CL_ prefix means 'sent from client to server'
*/


enum class NetMessage {
	SV_HANDSHAKE = 0,  //handshake response from server
	CL_HANDSHAKE, //client handshake
	SV_KICKED, //sent by server to inform client that he was kicked. socket disconnects immediately after that message was sent

	SV_LOADLEVEL, //sends level name to clients for load
	SV_ADDPLAYER, //signals that a player was added/connected
	SV_PLAYERIDENTIFY, //sent to each client and tells them what PlayerID does they have (in game.players vector)

	CL_PLAYERMOVE, //client should sent if they want to move the player
	SV_PLAYERMOVE, 

	CL_PLAYERSETSLOT, //updates client current slot on server, MUST be sent to avoid logic paradoxes
	SV_PLAYERSETSLOT,

	SV_PLAYERCHANGE, //change of player state

	SV_ADDWEAPON, //add weapon to player
	CL_BUYWEAPON, //try to buy weapon

	CL_SLOTCHANGE,

	CL_SHOOT, //SHOOT!
	SV_SHOTMADE, //send by server to inform that a shot was made
	SV_AMMOCHANGE,

	SV_RELOADGUN,

	SV_PHASE,
	SV_TIME,
	SV_ROUND,

	SV_MOVEPLAYER,
	SV_LIFEPLAYER,

	SV_ADDZOMBIE,
	SV_MOVEZOMBIE,
	SV_CHANGEZOMBIE,
	SV_KILLZOMBIE,

	SV_GOLD,

	CL_DOACTION,
	SV_CHATMSG,

	SV_OPENDOOR,

	CL_PING,
	SV_PONG,

	//DEBUG
	DBG_SPAWNZOMBIE,
	DBG_MOVEZOMBIES,
};

//signature, used in handshake
const static std::string SERVER_HANDSHAKE = "00xZBREAKOUT_SERVERx00";

class GameProtocol {
	public:
		static void sendServerHandshake(sf::TcpSocket& socket);
		static bool verifyServerHandshake(sf::Packet& packet);

		static void sendClientHandshake(sf::UdpSocket& socket, sf::IpAddress& ip, std::string nickname);
		static std::string verifyClientHandshake(sf::Packet& packet);

		static void sendLevel(sf::TcpSocket& socket, std::string name);

		static sf::Packet addPlayerPacket(std::string name);
		static sf::Packet identifyPlayerPacket(PlayerID id);

		static sf::Packet clientMove(sf::Vector2f dir);
		static sf::Packet serverMove(PlayerID id, sf::Vector2f pos);

		static sf::Packet playerUpdate(PlayerID id, Player& player);
	private:
		static void sendPacket(sf::TcpSocket& socket, sf::Packet& packet);
};

//sf::Packet overrides, used as convinience

sf::Packet& operator <<(sf::Packet& packet, const NetMessage& msg);
sf::Packet& operator >>(sf::Packet& packet, NetMessage& msg);

sf::Packet& operator >>(sf::Packet& packet, sf::Vector2f& arg);
sf::Packet& operator <<(sf::Packet& packet, const sf::Vector2f& arg);


sf::Packet& operator >>(sf::Packet& packet, Player& arg);
sf::Packet& operator <<(sf::Packet& packet, Player& arg);

sf::Packet& operator >>(sf::Packet& packet, Bullet& arg);
sf::Packet& operator <<(sf::Packet& packet, Bullet& arg);

sf::Packet& operator >>(sf::Packet& packet, ZombieType& arg);
sf::Packet& operator <<(sf::Packet& packet, ZombieType arg);

sf::Packet& operator >>(sf::Packet& packet, Zombie& arg);
sf::Packet& operator <<(sf::Packet& packet, Zombie& arg);

sf::Packet& operator >>(sf::Packet& packet, sf::Color& arg);
sf::Packet& operator <<(sf::Packet& packet, sf::Color& arg);

#endif