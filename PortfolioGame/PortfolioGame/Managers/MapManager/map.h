#pragma once
#include "../../Components/Base/game_object.h"
class Map : public GameObject
{
public:
	Map() = default;
	~Map() = default;
	// GameObject을(를) 통해 상속됨
	virtual int ReadyGameObject() override;

	virtual void UpdateGameObject(const float deltaTime) override;

	virtual void RenderGameObject(HDC hdc) override;

	virtual void LateUpdateGameObject() override;
private:


};

