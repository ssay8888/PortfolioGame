#pragma once
#include "../Base/game_object.h"
class Player :
    public GameObject
{
public:
    Player();
    ~Player();

    virtual int ReadyGameObject() override;
    virtual void UpdateActor(const float deltaTime) override;
    virtual void RenderGameObject(HDC hdc) override;
    virtual void LateUpdateGameObject() override;


private:
    uint16_t _skinId;
    
};