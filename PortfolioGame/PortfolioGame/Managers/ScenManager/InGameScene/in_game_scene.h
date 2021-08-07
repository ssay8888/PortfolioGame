#pragma once
#include "../scene.h"
class GameMouse;
class InGameScene :
    public Scene
{
public:
    InGameScene();
    ~InGameScene();
    // Scene��(��) ���� ��ӵ�
    virtual int ReadyScene() override;
    virtual void UpdateScene() override;
    virtual void LateUpdateScene() override;
    virtual void RenderScene(HDC hdc) override;
    virtual void ReleaseScene() override;

    static GameMouse* GetMouse();
private:
    static GameMouse* _mouse;
};

