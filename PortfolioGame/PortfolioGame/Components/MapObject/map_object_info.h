#pragma once
#include "../../Components/Base/game_object.h"
class MapObjectInfo : 
	public GameObject
{
public:
	MapObjectInfo(uint8_t layer);
	~MapObjectInfo();

	// GameObject��(��) ���� ��ӵ�
	virtual int ReadyGameObject() override;
	virtual void UpdateGameObject(const float deltaTime) override;
	virtual void RenderGameObject(HDC hdc) override;
	virtual void LateUpdateGameObject() override;


private:

};