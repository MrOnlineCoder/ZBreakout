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
	SV_HANDSHAKE - sent by server immidiately after client connection
	CL_HANDSHAKE "nickname" "version" - client's response to SV_HANDSHAKE.
	SV_KICKED "reason" - kick client
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

enum class NetMessage {
	SV_HANDSHAKE = 0,
	CL_HANDSHAKE,
	SV_KICKED,

	SV_PHASE,
	SV_TIME,
	SV_ROUND,

	SV_MOVEPLAYER,
	SV_LIFEPLAYER,

	SV_ADDZOMBIE,
	SV_MOVEZOMBIE,
	SV_KILLZOMBIE
};

#endif