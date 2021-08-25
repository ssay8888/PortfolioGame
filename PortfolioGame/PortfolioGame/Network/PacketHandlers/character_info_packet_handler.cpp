#include "pch.h"
#include "character_info_packet_handler.h"

#include "../client_session.h"

#include "../../Components/MapObject/Player/player.h"

CharacterInfoPacketHandler::~CharacterInfoPacketHandler()
{
}

void CharacterInfoPacketHandler::HandlePacket(ClientSession* scoket, InPacket* packet)
{
	auto name = packet->DecodeStr();
	scoket->GetPlayer()->SetName(name);
}
