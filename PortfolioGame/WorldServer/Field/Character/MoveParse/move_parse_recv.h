#pragma once

class MoveParseRecv
{
public:
	enum class PlayerMotion { kStand, kWalk, kJump, kRope, kLadder, kProne };
	MoveParseRecv();
	~MoveParseRecv();
	void SetX(int x);
	void SetY(int y);
	void SetFacingDirection(bool direction);
	void SetMotion(PlayerMotion motion);
	int32_t GetX() const;
	int32_t GetY() const;
	bool GetFacingDirection() const;
	PlayerMotion GetMotion() const;

private:
	int _x;
	int _y;
	bool _facing_direction;
	PlayerMotion _motion;
};

