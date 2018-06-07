/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

/*
	ZBreakout Network Game Protocol:

	Types:
	"string"
	%number%
	#vector (position) #
	@enum@

	Format:
	<Network Message Name> ... args ... - optional description 

	* Connection *
	SV_HANDSHAKE "server ID" - sent by server immidiately after client connection
	CL_HANDSHAKE "nickname" "version" - client's response to SV_HANDSHAKE.
	SV_KICKED "reason" - kick client
	SV_LOADLEVEL "levelname" - load level. should be sent after successful handshake
	SV_ADDPLAYER "name" - add player to client side.

	* Game State *
	SV_PHASE @phase@ - sets phase (LOBBY, FIGHT, COOLDOWN, END)
	SV_TIME %phase_time% %total_time%
	SV_ROUND %number% - tells about new round

	* Players *
	SV_MOVEPLAYER %pid% %x% %y% - move player
	SV_LIFEPLAYER %pid% @alive/dead@
	

	* Zombies *
	SV_ADDZOMBIE %id% @type@ %x% %y%
	SV_MOVEZOMBIE %id% %x% %y%
	SV_KILLZOMBIE %id%


*/

#ifndef GAMEPROTOCOL_H
#define GAMEPROTOCOL_H

#include <SFML/Network.hpp>

#include "../Logger.h"
#include "Constants.h"
#include "Player.h"

enum class NetMessage {
	SV_HANDSHAKE = 0,
	CL_HANDSHAKE,
	SV_KICKED,

	SV_LOADLEVEL,
	SV_ADDPLAYER,
	SV_PLAYERIDENTIFY,

	CL_PLAYERMOVE,
	SV_PLAYERMOVE,

	CL_PLAYERSETSLOT,
	SV_PLAYERSETSLOT,

	SV_PLAYERCHANGE,

	SV_ADDWEAPON,
	CL_BUYWEAPON,

	CL_SLOTCHANGE,

	CL_SHOOT,
	SV_SHOTMADE,

	SV_RELOADGUN,

	SV_PHASE,
	SV_TIME,
	SV_ROUND,

	SV_MOVEPLAYER,
	SV_LIFEPLAYER,

	SV_ADDZOMBIE,
	SV_MOVEZOMBIE,
	SV_KILLZOMBIE
};

const static std::string SERVER_HANDSHAKE = "00xZBREAKOUT_SERVERx00";

class GameProtocol {
	public:
		static void sendServerHandshake(sf::TcpSocket& socket);
		static bool verifyServerHandshake(sf::Packet& packet);

		static void sendClientHandshake(sf::TcpSocket& socket, std::string nickname);
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

sf::Packet& operator <<(sf::Packet& packet, const NetMessage& msg);
sf::Packet& operator >>(sf::Packet& packet, NetMessage& msg);

sf::Packet& operator >>(sf::Packet& packet, sf::Vector2f& arg);
sf::Packet& operator <<(sf::Packet& packet, const sf::Vector2f& arg);


sf::Packet& operator >>(sf::Packet& packet, Player& arg);
sf::Packet& operator <<(sf::Packet& packet, Player& arg);

sf::Packet& operator >>(sf::Packet& packet, Bullet& arg);
sf::Packet& operator <<(sf::Packet& packet, Bullet& arg);

#endif