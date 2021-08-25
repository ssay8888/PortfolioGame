#pragma once

class ClientSession;
class InPacket;

class ClientPacketHandler {
public:
	ClientPacketHandler() = default;
	virtual ~ClientPacketHandler() = default;

	virtual void HandlePacket(ClientSession* socket, InPacket* packet) = 0;

};
