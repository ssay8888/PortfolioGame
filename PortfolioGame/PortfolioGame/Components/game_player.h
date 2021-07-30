#pragma once
#include "game_object.h"

class GamePlayer : public GameObject
{
public:
	GamePlayer();
    virtual ~GamePlayer();
	void UpdateActor(const float deltaTime) override;
};

