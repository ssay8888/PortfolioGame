#include "../../../../pch.h"
#include "ai_movement.h"

AiMovement::AiMovement(float speed) :
	_move_range(static_cast<float>(rand() % 250)),
	_facing_direction(rand() % 2),
	_speed(speed),
	_is_stand(false)
{
	ResetMoveRange();
}

float AiMovement::Moveing()
{
	if (_is_stand)
	{
		if (GetTickCount64() > _stand_tick + 3000)
		{
			ResetMoveRange();
		}
		return 0;
	} else if (!_facing_direction)
	{
		if (_move_range + _speed > 0)
		{
			ResetMoveRange();
		}
		_move_range += _speed;
		return -_speed;
	}
	if (_move_range - _speed < 0)
	{
		ResetMoveRange();
	}
	_move_range -= _speed;
	return _speed;
}

void AiMovement::SetFacingDirection(bool facing)
{
	_facing_direction = facing;
}

bool AiMovement::GetFacingDirection()
{
	return _facing_direction;
}

void AiMovement::GainMoveRange(float value)
{
	_move_range += value;
}

float AiMovement::GetMoveRange() const
{
	return _move_range;
}

void AiMovement::ResetMoveRange()
{
	if (rand() % 10 == 1 || true)
	{
		_is_stand = true;
		_stand_tick = GetTickCount64();
		return;
	}
	_facing_direction = rand() % 2;
	_move_range = static_cast<float>(rand() % 1000);
	if (!_facing_direction)
		_move_range *= -1;

	_is_stand = false;
	_stand_tick = 0;
}
