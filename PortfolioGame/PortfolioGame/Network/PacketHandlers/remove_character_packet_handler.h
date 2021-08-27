#pragma once
#include "../PacketHandlerManager/client_packet_handler.h"
class RemoveCharacterPacketHandler : public ClientPacketHandler
{
public:
	~RemoveCharacterPacketHandler() override;
	void HandlePacket(ClientSession* socket, InPacket* packet) override;
};

