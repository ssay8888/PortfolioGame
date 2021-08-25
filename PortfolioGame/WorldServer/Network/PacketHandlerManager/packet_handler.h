#pragma once

class WorldClientSession;
class InPacket;

class PacketHandler {
public:
	PacketHandler() = default;
	virtual ~PacketHandler() = default;

	virtual void HandlePacket(std::shared_ptr<WorldClientSession> socket, std::shared_ptr<InPacket> packet) = 0;

};
