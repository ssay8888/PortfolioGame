#pragma once
#include "../Network/PacketHandlerManager/packet_handler.h"
class Character;

class LoginHandler : public PacketHandler
{
public:
	void HandlePacket(std::shared_ptr<WorldClientSession> scoket, std::shared_ptr<InPacket> packet) override;

	std::shared_ptr<OutPacket> CreateLoginResponsePacket(std::shared_ptr<Character> character) const;
	std::shared_ptr<OutPacket> CreateCharacterInfoPacket(std::shared_ptr<Character> character) const;
	std::shared_ptr<OutPacket> CreateSpawnPlayerPacket(std::shared_ptr<Character> character) const;
};

