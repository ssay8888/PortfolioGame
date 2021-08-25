#pragma once
#include "../PacketHandlerManager/client_packet_handler.h"
class MovePlayerPacketHandler :
    public ClientPacketHandler
{
public:
	~MovePlayerPacketHandler() override;
	void HandlePacket(ClientSession* socket, InPacket* packet) override;
};

