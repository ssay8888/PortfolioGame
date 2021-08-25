#pragma once
#include "../PacketHandlerManager/client_packet_handler.h"
class SpawnPlayerPacketHandler : public ClientPacketHandler
{
public:
	~SpawnPlayerPacketHandler() override;
	void HandlePacket(ClientSession* scoket, InPacket* packet) override;
};

