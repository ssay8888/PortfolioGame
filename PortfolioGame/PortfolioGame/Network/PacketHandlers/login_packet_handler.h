#pragma once

#include "../PacketHandlerManager/client_packet_handler.h"
class LoginPacketHandler : public ClientPacketHandler
{
public:
	~LoginPacketHandler() override;
	void HandlePacket(ClientSession* socket, InPacket* packet) override;
};