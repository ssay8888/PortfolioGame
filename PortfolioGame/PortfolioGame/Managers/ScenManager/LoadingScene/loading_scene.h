#pragma once
#include "../scene.h"
class MyBitmap;
class LoadingScene : public Scene
{
public:

    LoadingScene();
    ~LoadingScene();
    // Scene��(��) ���� ��ӵ�
    virtual int ReadyScene() override;

    virtual void UpdateScene() override;

    virtual void LateUpdateScene() override;

    virtual void RenderScene(HDC hdc) override;

    virtual void ReleaseScene() override;

private:
    std::vector<MyBitmap*> _logoImages;
    MyBitmap* _lastLogo;
    int _logoFrameCount;

    ULONGLONG _tick;
    const uint32_t _frameDelay;
    const uint32_t _WindowCX;
    const uint32_t _WindowCY;



};

