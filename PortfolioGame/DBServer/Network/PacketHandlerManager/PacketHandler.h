#pragma once

class DatabaseClientSession;
class InPacket;

class PacketHandler {
public:
	PacketHandler() = default;
	virtual ~PacketHandler() = default;

	virtual void HandlePacket(std::shared_ptr<DatabaseClientSession> socket, std::shared_ptr<InPacket> packet) = 0;

};
