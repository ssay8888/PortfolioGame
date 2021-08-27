#include "../pch.h"
#include "packet_creator.h"
#include "../Components/MapObject/Player/MoveParse/move_parse.h"

OutPacket* PacketCreator::CreatorMovePlayer(std::list<std::shared_ptr<MoveParse>> list_move)
{
	OutPacket* out_packet = new OutPacket();

	out_packet->Encode1(static_cast<int>(::opcode::ClientOpcode::kMovePlayer));
	out_packet->Encode4(static_cast<int>(list_move.size()));
  for (auto& move : list_move)
  {
	  out_packet->Encode1(move->GetFacingDirection());
	  out_packet->Encode1(static_cast<char>(move->GetMotion()));
	  out_packet->Encode4(move->GetX());
	  out_packet->Encode4(move->GetY());
  }
	return out_packet;
}
