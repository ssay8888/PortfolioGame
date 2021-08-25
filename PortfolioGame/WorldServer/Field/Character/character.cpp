#include "../../pch.h"
#include "character.h"
Character::Character(): _id(0),
                        _pos() {}

Character::~Character() {}
void Character::SetId(int32_t id)
{
	_id = id;
}
int32_t Character::GetId() const
{
	return _id;
}

void Character::SetPos(POINT pos)
{
	_pos = pos;
}
POINT Character::GetPos() const
{
	return _pos;
}
