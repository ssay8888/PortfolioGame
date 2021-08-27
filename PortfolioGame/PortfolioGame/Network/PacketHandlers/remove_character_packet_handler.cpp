#include "../../pch.h"
#include "remove_character_packet_handler.h"
#include "../../Managers/MapManager/map_manager.h"
#include "../../Managers/MapManager/Map/map_instance.h"


RemoveCharacterPacketHandler::~RemoveCharacterPacketHandler()
{
}

void RemoveCharacterPacketHandler::HandlePacket(ClientSession* socket, InPacket* packet)
{
	const int32_t remove_id = packet->Decode4();
	const auto now_map      = MapManager::GetInstance()->GetNowMap();
  if (now_map != nullptr)
  {
	  now_map->RemovePlayer(remove_id);
  }
}
