#include "../../../pch.h"
#include "move_parse_recv.h"
MoveParseRecv::MoveParseRecv(): _x(0),
                                _y(0),
                                _facing_direction(false),
                                _motion() {}

MoveParseRecv::~MoveParseRecv() {}
void MoveParseRecv::SetX(int x)
{
	_x = x;
}
void MoveParseRecv::SetY(int y)
{
	_y = y;
}
void MoveParseRecv::SetFacingDirection(bool direction)
{
	_facing_direction = direction;
}
void MoveParseRecv::SetMotion(PlayerMotion motion)
{
	_motion = motion;
}
int32_t MoveParseRecv::GetX() const
{
	return _x;
}
int32_t MoveParseRecv::GetY() const
{
	return _y;
}
bool MoveParseRecv::GetFacingDirection() const
{
	return _facing_direction;
}
MoveParseRecv::PlayerMotion MoveParseRecv::GetMotion() const
{
	return _motion;
}
