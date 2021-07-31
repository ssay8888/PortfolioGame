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
    virtual void UpdateGameObject(const float deltaTime) override;
    virtual void RenderGameObject(HDC hdc) override;
    virtual void LateUpdateGameObject() override;

    void SetFrameThis(SkinInfo* frame);
    SkinInfo* GetFrameThis();

    std::vector<SkinInfo*> FindSkinFrame() const;
    void AddFrame(SkinInfo* item);

    void ChangeFrameState(std::string frame);
    char* GetFrameState();

    uint16_t GetFrameNummber(uint16_t remain);

private:
    void LoadCharacterFrame(std::string frameName, uint16_t frameCount = 3);
    void RenderCharacter(HDC hdc);
private:
    uint16_t _skinId;
    uint16_t _frameNummber;
    SkinInfo* _frameThis;
    std::string _frameState;
    std::map<std::string, std::vector<SkinInfo*>> _skinFrames;
    std::vector<SkinInfo*> _itemFrames;
    uint64_t _frameTick;
};