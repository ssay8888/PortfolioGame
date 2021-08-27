#include "../pch.h"
#include "login_handler.h"

#include "../Field/Map/map.h"
#include "../Field/Map/map_factory.h"
#include "../Network/world_client_session.h"
#include "../Field/Character/character.h"
#include "../Network/world_server.h"

void LoginHandler::HandlePacket(std::shared_ptr<WorldClientSession> scoket, std::shared_ptr<InPacket> packet)
{
	std::cout << "로그인 처리" << std::endl;
	auto map = MapFactory::GetInstance()->FindMap(0);
	auto character = std::make_shared<Character>();
	character->SetId(packet->Decode1());
	map->InsertCharacter(character);
	scoket->SetCharacter(character);

	scoket->SendPacket(CreateLoginResponsePacket(character));
	scoket->SendPacket(CreateCharacterInfoPacket(character));
	scoket->SendPacket(CreateSpawnPlayerPacket(character), true);
	auto session_list = WorldServer::GetInstance()->GetSessionList();
	for (auto session_piar : session_list)
	{
		if (session_piar.second != scoket)
		{
		  if (session_piar.second->GetCharacter() != nullptr)
		  {
			  scoket->SendPacket(CreateSpawnPlayerPacket(session_piar.second->GetCharacter()));
		  }
		}
	}
}

std::shared_ptr<OutPacket> LoginHandler::CreateLoginResponsePacket(std::shared_ptr<Character> character) const
{
	std::shared_ptr<OutPacket> out_packet = std::make_shared<OutPacket>();
	out_packet->Encode1(static_cast<char>(::opcode::ServerOpcode::kLoginResponse));
	out_packet->Encode4(character->GetId());
	return out_packet;
}

std::shared_ptr<OutPacket> LoginHandler::CreateCharacterInfoPacket(const std::shared_ptr<Character> character) const
{
	std::shared_ptr<OutPacket> out_packet = std::make_shared<OutPacket>();
	out_packet->Encode1(static_cast<char>(::opcode::ServerOpcode::kCharacterInfo));
	if (character->GetId() == 0)
	{
		out_packet->EncodeStr("뿌에엥");
	}
	else
	{
		out_packet->EncodeStr("무야호");
	}
	return out_packet;
}

std::shared_ptr<OutPacket> LoginHandler::CreateSpawnPlayerPacket(
	const std::shared_ptr<Character> character) const
{
	std::shared_ptr<OutPacket> out_packet = std::make_shared<OutPacket>();
	out_packet->Encode1(static_cast<char>(::opcode::ServerOpcode::kSpawnPlayer));
	out_packet->Encode4(character->GetId());
	return out_packet;
}
