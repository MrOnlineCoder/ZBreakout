/*
ZBreakout Project

Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
All rights reserved.

Only original author is allowed to edit and share this file.
Unauthorized copying or editing this file, via any medium is strictly prohibited
Proprietary and confidential
*/

#include "GameProtocol.h"

void GameProtocol::sendServerHandshake(sf::TcpSocket& socket) {
	sf::Packet packet;
	packet << NetMessage::SV_HANDSHAKE << SERVER_HANDSHAKE;
	sendPacket(socket, packet);
}

bool GameProtocol::verifyServerHandshake(sf::Packet & packet) {
	std::string text;
	packet >> text;

	if (text != SERVER_HANDSHAKE) {
		return false;
	}

	return true;
}

void GameProtocol::sendClientHandshake(sf::TcpSocket & socket, std::string nickname) {
	sf::Packet packet;
	packet << NetMessage::CL_HANDSHAKE << nickname << Constants::VERSION;
	sendPacket(socket, packet);
}

std::string GameProtocol::verifyClientHandshake(sf::Packet & packet) {
	std::string name;
	std::string ver;

	packet >> name;
	packet >> ver;

	if (ver != Constants::VERSION) {
		return "";
	}

	return name;
}

void GameProtocol::sendLevel(sf::TcpSocket & socket, std::string name) {
	sf::Packet packet;
	packet << NetMessage::SV_LOADLEVEL << name;
	sendPacket(socket, packet);
}

sf::Packet GameProtocol::addPlayerPacket(std::string name) {
	sf::Packet packet;
	packet << NetMessage::SV_ADDPLAYER << name;
	return packet;
}

sf::Packet GameProtocol::identifyPlayerPacket(PlayerID id) {
	sf::Packet packet;
	packet << NetMessage::SV_PLAYERIDENTIFY << id;
	return packet;
}

sf::Packet GameProtocol::clientMove(sf::Vector2f dir) {
	sf::Packet packet;
	packet << NetMessage::CL_PLAYERMOVE << dir.x << dir.y;
	return packet;
}

sf::Packet GameProtocol::serverMove(PlayerID id, sf::Vector2f pos) {
	sf::Packet packet;
	packet << NetMessage::SV_PLAYERMOVE << id << pos;
	return packet;
}

sf::Packet GameProtocol::playerUpdate(PlayerID id, Player & player) {
	sf::Packet packet;
	packet << NetMessage::SV_PLAYERCHANGE << id << player;
	return packet;
}

void GameProtocol::sendPacket(sf::TcpSocket & socket, sf::Packet & packet) {
	sf::Socket::Status status = socket.send(packet);

	if (status == sf::Socket::Status::Error) {
		_LOG_.log("GameProtocol", "ERROR: Failed to send packet");
		return;
	}

	if (status == sf::Socket::Status::Partial) {
		socket.send(packet);
	}
}


sf::Packet& operator <<(sf::Packet& packet, const NetMessage& msg) {
	return packet << static_cast<int>(msg);
}

sf::Packet& operator >>(sf::Packet& packet, NetMessage& msg) {
	int m;
	packet >> m;
	msg = static_cast<NetMessage>(m);
	return packet;
}

sf::Packet& operator <<(sf::Packet& packet, const sf::Vector2f& arg) {
	return packet << arg.x << arg.y;
}

sf::Packet& operator >>(sf::Packet& packet, sf::Vector2f& arg) {
	packet >> arg.x >> arg.y;
	return packet;
}

sf::Packet& operator >>(sf::Packet& packet, Player& arg) {
	return packet >> arg.pos >> arg.direction;
}

sf::Packet& operator <<(sf::Packet& packet, Player& arg) {
	return packet << arg.pos << arg.direction;
}


sf::Packet& operator >>(sf::Packet& packet, Bullet& arg) {
	return packet >> arg.angle >> arg.damage >> arg.pos >> arg.shooter >> arg.type;
}

sf::Packet& operator <<(sf::Packet& packet, Bullet& arg) {
	return packet << arg.angle << arg.damage << arg.pos << arg.shooter << arg.type;
}

sf::Packet& operator >>(sf::Packet& packet, ZombieType& arg) {
	int val;
	packet >> val;
	arg = static_cast<ZombieType>(val);
	return packet;
}

sf::Packet& operator <<(sf::Packet& packet, ZombieType arg) {
	packet << static_cast<int>(arg);

	return packet;
}

sf::Packet& operator >>(sf::Packet& packet, Zombie& arg) {
	packet >> arg.pos >> arg.hp;
	return packet;
}

sf::Packet& operator <<(sf::Packet& packet, Zombie& arg) {
	packet << arg.pos << arg.hp;
	return packet;
}