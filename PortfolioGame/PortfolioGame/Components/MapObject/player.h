#pragma once
#include "../Base/game_object.h"

class SkinInfo;
class Player :
    public GameObject
{
public:
    Player(uint8_t layer = 5);
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
    const char* GetFrameState() const;

    uint16_t GetFrameNummber(uint16_t remain);

    void SetFacingDirection(uint8_t direction);
    uint8_t GetFacingDirection() const;

private:
    void LoadCharacterFrame(std::string frameName, uint16_t frameCount = 3);
    void RenderCharacter(HDC hdc);
private:
    uint16_t _skinId;
    uint16_t _frameNummber;
    bool _frameRevers;
    uint8_t _facingDirection;
    SkinInfo* _frameThis;
    std::string _frameState;
    std::map<std::string, std::vector<SkinInfo*>> _skinFrames;
    std::vector<SkinInfo*> _itemFrames;
    uint64_t _frameTick;

    HDC _memDC;
    HBITMAP _bitmap;
    HBITMAP _oldBitmap;
    HBITMAP _hBitmap;
};