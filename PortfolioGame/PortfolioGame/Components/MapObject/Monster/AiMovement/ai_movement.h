#pragma once
class AiMovement
{
public:
	AiMovement(float _speed);
	~AiMovement() = default;

	float Moveing();

	void SetFacingDirection(bool facing);
	bool GetFacingDirection();

	void GainMoveRange(float value);
	float GetMoveRange() const;

	void ResetMoveRange();
private:
	float _move_range;
	bool _facing_direction;
	bool _is_stand;
	uint64_t _stand_tick;
	float _speed;
};

