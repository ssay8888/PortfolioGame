#pragma once

class WorldClientSession;
class InPacket;

class PacketHandler {
public:
	PacketHandler() = default;
	virtual ~PacketHandler() = default;

	virtual void HandlePacket(WorldClientSession* scoket, InPacket* packet) = 0;

};
