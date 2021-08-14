#pragma once
#include "Base/game_object.h"
class KeyManager;
class GameMouse :
    public GameObject
{
    enum class MouseState { kBasic, kClick, kDrag, kIsButton, kEnd };
private:
    virtual int ReadyGameObject() override;
    virtual void UpdateGameObject(const float deltaTime) override;
    virtual void RenderGameObject(HDC hdc) override;
    virtual void LateUpdateGameObject() override;

public:
    GameMouse();
    ~GameMouse();

    POINT GetPoint() const;
private:
    uint32_t _frameCount;
    MouseState _state;
    std::map<MouseState, std::vector<std::shared_ptr<MyBitmap>>> _mouseImage;
    POINT _cursorPos;
    KeyManager* _key_manager;
};

