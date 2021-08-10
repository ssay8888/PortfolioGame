#pragma once
#include "../Base/game_object.h"
class MyBitmap;
class AniMapObject : public
	GameObject
{
public:
	AniMapObject();
	~AniMapObject();
private:
	// GameObject을(를) 통해 상속됨
	virtual int ReadyGameObject() override;
	virtual void UpdateGameObject(const float deltaTime) override;
	virtual void RenderGameObject(HDC hdc) override;
	virtual void LateUpdateGameObject() override;

public:
	void InsertAni(ObjectPos pos, MyBitmap* image);
private:
	std::vector<std::pair<ObjectPos, MyBitmap*>> _ani_image;
	uint8_t _frame_number;
	uint64_t _frame_tick;

};

