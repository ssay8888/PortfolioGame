#include "../../pch.h"
#include "login_packet_handler.h"

#include "../client_session.h"

#include "../../Components/MapObject/Player/player.h"

#include "../../Managers/ScenManager/scene_manager.h"
#include "../../Managers/SoundManager/sound_manager.h"

LoginPacketHandler::~LoginPacketHandler()
{
}

void LoginPacketHandler::HandlePacket(ClientSession* socket, InPacket* packet)
{
	socket->GetPlayer()->SetObjectId(packet->Decode4());
	SoundManager::GetInstance()->PlaySound(L"ScrollUp.mp3", SoundManager::kPlayer);
	SoundManager::GetInstance()->StopSound(SoundManager::kBgm);
	SceneManager::GetInstance()->SceneChange(SceneManager::SceneState::kInGame);
}
