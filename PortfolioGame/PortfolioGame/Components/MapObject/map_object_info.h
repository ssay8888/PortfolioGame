#pragma once
#include "../../Components/Base/game_object.h"
class MapObjectInfo : 
	public GameObject
{
public:
	MapObjectInfo(uint8_t layer);
	~MapObjectInfo();

	// GameObject을(를) 통해 상속됨
	virtual int ReadyGameObject() override;
	virtual void UpdateGameObject(const float deltaTime) override;
	virtual void RenderGameObject(HDC hdc) override;
	virtual void LateUpdateGameObject() override;


private:

};