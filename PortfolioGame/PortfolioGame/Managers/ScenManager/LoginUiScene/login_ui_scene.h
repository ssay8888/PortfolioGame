#pragma once
#include "../scene.h"
class MyBitmap;
class LoginUiScene  : public Scene
{
public:
	LoginUiScene();
	~LoginUiScene();
	// Scene��(��) ���� ��ӵ�
	virtual int ReadyScene() override;
	virtual void UpdateScene() override;
	virtual void LateUpdateScene() override;
	virtual void RenderScene(HDC hdc) override;
	virtual void ReleaseScene() override;

private:

	MyBitmap* _loginUiImage;
};

