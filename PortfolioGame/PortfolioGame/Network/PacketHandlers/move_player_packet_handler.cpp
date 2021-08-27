#include "../../pch.h"
#include "move_player_packet_handler.h"

#include "../../Components/MapObject/Player/player.h"
#include "../../Components/MapObject/Player/MoveParse/move_parse.h"

#include "../../Managers/MapManager/map_manager.h"
#include "../../Managers/MapManager/Map/map_instance.h"

MovePlayerPacketHandler::~MovePlayerPacketHandler()
{
}

void MovePlayerPacketHandler::HandlePacket(ClientSession* socket, InPacket* packet)
{
	const int32_t id = packet->Decode4();
	Player* player = nullptr;
	if (MapManager::GetInstance()->GetNowMap() != nullptr)
	{
		player = MapManager::GetInstance()->GetNowMap()->FindPlayer(id);
	}
  if (player == nullptr)
  {
	  return;
  }
	const int32_t size = packet->Decode4();
	for (int i = 0; i < size; ++i)
	{
		const std::shared_ptr<MoveParse> move = std::make_shared<MoveParse>(player, ::ObjectType::PlayerMotion::kStand);
		move->SetFacingDirection(packet->Decode1());
		move->SetMotion(static_cast<ObjectType::PlayerMotion>(packet->Decode1()));
		move->SetX(packet->Decode4());
		move->SetY(packet->Decode4());
		player->InsertMoveParse(move);
	}
}
