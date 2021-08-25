#include "../pch.h"
#include "move_player_handler.h"

#include "../Field/Character/character.h"

#include "../Network/world_client_session.h"

MovePlayerHandler::~MovePlayerHandler()
{
}

void MovePlayerHandler::
HandlePacket(std::shared_ptr<WorldClientSession> socket, std::shared_ptr<InPacket> packet)
{
	POINT pos{ packet->Decode4(), packet->Decode4() };
	socket->GetCharacter()->SetPos(pos);
	socket->SendPacket(CreateMovePlayerPacket(socket), true);
}

std::shared_ptr<OutPacket> MovePlayerHandler::CreateMovePlayerPacket(std::shared_ptr<WorldClientSession> socket)
{
	std::shared_ptr<OutPacket> out_packet = std::make_shared<OutPacket>();
	out_packet->Encode1(static_cast<int>(::opcode::ServerSend::kMovePlayer));
	out_packet->Encode4(socket->GetCharacter()->GetId());
	out_packet->Encode4(socket->GetCharacter()->GetPos().x);
	out_packet->Encode4(socket->GetCharacter()->GetPos().y);
	return out_packet;
}
