#pragma once
class Monster;

class AiMovement
{
public:
	AiMovement(float _speed);
	~AiMovement() = default;

	float Moveing();

	void SetFacingDirection(bool facing);
	bool GetFacingDirection() const;

	void GainMoveRange(float value);
	float GetMoveRange() const;

	void SetPartner(Monster* partner);
	Monster* GetPartner();
	void ResetMoveRange();
private:
	float _move_range;
	bool _facing_direction;
	bool _is_stand;
	uint64_t _stand_tick;
	float _speed;
	Monster* _partner;
};

