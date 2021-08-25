#include "../../pch.h"
#include "spawn_player_packet_handelr.h"
#include "../../Managers/MapManager/map_manager.h"
#include "../../Components/MapObject/Player/player.h"

#include "../../Managers/MapManager/Map/map_instance.h"

SpawnPlayerPacketHandler::~SpawnPlayerPacketHandler()
{
}

void SpawnPlayerPacketHandler::HandlePacket(ClientSession* scoket, InPacket* packet)
{
	int32_t id =	packet->Decode4();

	const auto object_manager = MapManager::GetInstance();
	const auto object         = new Player();
	object->SetObjectId(id);
  if (object_manager->GetNowMap() != nullptr)
  {
	  object_manager->GetNowMap()->AddGameObject(object);
	  object_manager->GetNowMap()->InsertPlayer(object);
  }
	std::cout << "½ºÆùÇÃ " << id << std::endl;
}
