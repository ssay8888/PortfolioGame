#include "../../../../pch.h"
#include "move_parse.h"

#include "../player.h"

MoveParse::MoveParse(Player* player, ObjectType::PlayerMotion motion) : _x(static_cast<int>(player->GetInfo().x)),
                                       _y(static_cast<int>(player->GetInfo().y)),
                                       _facing_direction(player->GetFacingDirection()),
                                       _motion(motion),
                                       _player(player)
{
	
}

MoveParse::~MoveParse()
{
	
}
void MoveParse::SetX(int x)
{
	_x = x;
}
void MoveParse::SetY(int y) {
	_y = y;
}
void MoveParse::SetFacingDirection(bool direction)
{
	_facing_direction = direction;
}
void MoveParse::SetMotion(ObjectType::PlayerMotion motion)
{
	_motion = motion;
}
void MoveParse::SetPlayer(Player* player)
{
	_player = player;
}
int32_t MoveParse::GetX() const
{
	return _x;
}
int32_t MoveParse::GetY() const
{
	return _y;
}
bool MoveParse::GetFacingDirection() const
{
	return _facing_direction;
}
ObjectType::PlayerMotion MoveParse::GetMotion() const
{
	return _motion;
}
Player* MoveParse::GetPlayer()
{
	return _player;
}
