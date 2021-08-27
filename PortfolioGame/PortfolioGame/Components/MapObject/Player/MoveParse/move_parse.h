#pragma once
class Player;

class MoveParse
{
public:
	MoveParse(Player* player, ObjectType::PlayerMotion motion);
	~MoveParse();
	void SetX(int x);
	void SetY(int y);
	void SetFacingDirection(bool direction);
	void SetMotion(ObjectType::PlayerMotion motion);
	void SetPlayer(Player* player);
	int32_t GetX() const;
	int32_t GetY() const;
	bool GetFacingDirection() const;
	ObjectType::PlayerMotion GetMotion() const;
	Player* GetPlayer();

private:
	int _x;
	int _y;
	bool _facing_direction;
	ObjectType::PlayerMotion _motion;
	Player* _player;
};

