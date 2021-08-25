#include "../../pch.h"
#include "login_packet_handler.h"

#include "../client_session.h"

#include "../../Components/MapObject/Player/player.h"

#include "../../Managers/ScenManager/scene_manager.h"

LoginPacketHandler::~LoginPacketHandler()
{
}

void LoginPacketHandler::HandlePacket(ClientSession* socket, InPacket* packet)
{
	socket->GetPlayer()->SetObjectId(packet->Decode4());
	SceneManager::GetInstance()->SceneChange(SceneManager::SceneState::kInGame);
}
