#include "../../pch.h"
#include "monster_parts.h"

MonsterParts::MonsterParts() :
	_delay(0),
	_origin({}),
	_head({}),
	_rect({}),
	_image(nullptr)
{

}

MonsterParts::~MonsterParts()
{
}

uint32_t MonsterParts::GetDelay() const
{
	return _delay;
}

void MonsterParts::SetDelay(const uint32_t delay)
{
	_delay = delay;
}

POINT MonsterParts::GetOriginPos() const
{
	return _origin;
}

void MonsterParts::SetOriginPos(const POINT origin)
{
	_origin = origin;
}

void MonsterParts::SetOriginPosX(const LONG x)
{
	_origin.x = x;
}

void MonsterParts::SetOriginPosY(const LONG y)
{
	_origin.y = y;
}

POINT MonsterParts::GetHeadPos() const
{
	return _head;
}

void MonsterParts::SetHeadPos(const POINT headpos)
{
	_head = headpos;
}

void MonsterParts::SetHeadPosX(const LONG x)
{
	_head.x = x;
}

void MonsterParts::SetHeadPosY(const LONG y)
{
	_head.y = y;
}

RECT MonsterParts::GetRect() const
{
	return _rect;
}

void MonsterParts::SetRect(const RECT rect)
{
	_rect = rect;
}

void MonsterParts::SetRectLeft(const LONG left)
{
	_rect.left = left;
}

void MonsterParts::SetRectTop(const LONG top)
{
	_rect.top = top;

}

void MonsterParts::SetRectRight(const LONG right)
{
	_rect.right = right;
}

void MonsterParts::SetRectBottom(const LONG bottom)
{
	_rect.bottom = bottom;
}

std::string MonsterParts::GetUol() const
{
	return _uol;
}

void MonsterParts::SetUol(std::string uol)
{
	_uol = uol;
}

void MonsterParts::SetImage(const std::shared_ptr<MyBitmap*> image)
{
	_image = image;
}

std::shared_ptr<MyBitmap*> MonsterParts::GetImage() const
{
	return _image;
}

void MonsterParts::SetPartner(std::shared_ptr<MonsterMovement*> partner)
{
	_partner = partner;
}

std::shared_ptr<MonsterMovement*> MonsterParts::GetPartner() const
{
	return _partner;
}
