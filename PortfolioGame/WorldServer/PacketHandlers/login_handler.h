#pragma once
#include "../Network/PacketHandlerManager/packet_handler.h"
class LoginHandler : public PacketHandler
{
public:
	void HandlePacket(WorldClientSession* scoket, InPacket* packet) override;
};

