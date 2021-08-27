#pragma once
class MoveParse;
class Player;

class PacketCreator
{
public:
	static OutPacket* CreatorMovePlayer(std::list<std::shared_ptr<MoveParse>> list_move);
};

