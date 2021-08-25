#include "../../pch.h"
#include "move_player_packet_handler.h"

#include "../../Components/MapObject/Player/player.h"

#include "../../Managers/MapManager/map_manager.h"
#include "../../Managers/MapManager/Map/map_instance.h"

MovePlayerPacketHandler::~MovePlayerPacketHandler()
{
}

void MovePlayerPacketHandler::HandlePacket(ClientSession* socket, InPacket* packet)
{
	int32_t id = packet->Decode4();
	int32_t x = packet->Decode4();
	int32_t y = packet->Decode4();

  if (MapManager::GetInstance()->GetNowMap() != nullptr)
  {
	  auto player = MapManager::GetInstance()->GetNowMap()->FindPlayer(id);
	  if (player != nullptr)
	  {
		  auto info = player->GetInfo();
		  info.x = static_cast<float>(x);
		  info.y = static_cast<float>(y);
		  player->SetInfo(info);
	  }
  }
}
