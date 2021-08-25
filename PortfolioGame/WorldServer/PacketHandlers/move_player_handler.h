#pragma once
#include "../Network/PacketHandlerManager/packet_handler.h"
class Character;

class MovePlayerHandler : public PacketHandler
{
public:
	~MovePlayerHandler() override;
	void HandlePacket(std::shared_ptr<WorldClientSession> socket, std::shared_ptr<InPacket> packet) override;

	std::shared_ptr<OutPacket> CreateMovePlayerPacket(std::shared_ptr<WorldClientSession> socket);
};

