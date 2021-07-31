#pragma once
#include "../Base/game_object.h"

class SkinInfo;
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


    void SetFrameThis(SkinInfo* frame);
    SkinInfo* GetFrameThis();

    std::vector<SkinInfo*> FindSkinFrame() const;
    void AddFrame(SkinInfo* item);
private:
    uint16_t _skinId;
    uint16_t _frameNummber;
    SkinInfo* _frameThis;
    std::map<std::string, std::vector<SkinInfo*>> _skinFrames;
    std::vector<SkinInfo*> _itemFrames;
};