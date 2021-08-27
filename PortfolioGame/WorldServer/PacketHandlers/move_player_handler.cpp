#include "../pch.h"
#include "move_player_handler.h"

#include "../Field/Character/character.h"
#include "../Field/Character/MoveParse/move_parse_recv.h"

#include "../Network/world_client_session.h"
#
MovePlayerHandler::~MovePlayerHandler()
{
}

void MovePlayerHandler::
HandlePacket(std::shared_ptr<WorldClientSession> socket, std::shared_ptr<InPacket> packet)
{
	int size = packet->Decode4();

	std::list<std::shared_ptr<MoveParseRecv>> list;
	for (int i = 0; i < size; ++i)
	{
		auto parse = std::make_shared<MoveParseRecv>();
		parse->SetFacingDirection(packet->Decode1());
		parse->SetMotion(static_cast<MoveParseRecv::PlayerMotion>(packet->Decode1()));
		parse->SetX(packet->Decode4());
		parse->SetY(packet->Decode4());
		POINT pos{ parse->GetX(), parse->GetY()};
		socket->GetCharacter()->SetPos(pos);
		list.emplace_back(parse);
	}
	socket->SendPacket(CreateMovePlayerPacket(socket, list), true);
	//POINT pos{ packet->Decode4(), packet->Decde4() };o
	//socket->GetCharacter()->SetPos(pos);
	//socket->SendPacket(CreateMovePlayerPacket(socket), true);

  /*
	out_packet->Encode1(static_cast<int>(::opcode::ClientSend::kMovePlayer));
	out_packet->Encode4(static_cast<int>(list_move.size()));
  for (auto& move : list_move)
  {
	  out_packet->Encode1(move->GetFacingDirection());
	  out_packet->Encode1(static_cast<char>(move->GetMotion()));
	  out_packet->Encode4(move->GetX());
	  out_packet->Encode4(move->GetY());
  }*/
}

std::shared_ptr<OutPacket> MovePlayerHandler::CreateMovePlayerPacket(std::shared_ptr<WorldClientSession> socket, std::list<std::shared_ptr<MoveParseRecv>> list) const
{
	std::shared_ptr<OutPacket> out_packet = std::make_shared<OutPacket>();
	out_packet->Encode1(static_cast<int>(::opcode::ServerOpcode::kMovePlayer));
	out_packet->Encode4(socket->GetCharacter()->GetId());
	out_packet->Encode4(static_cast<int>(list.size()));

  for (auto& move : list)
  {
	  out_packet->Encode1(move->GetFacingDirection());
	  out_packet->Encode1(static_cast<char>(move->GetMotion()));
	  out_packet->Encode4(move->GetX());
	  out_packet->Encode4(move->GetY());
  }
	return out_packet;
}
