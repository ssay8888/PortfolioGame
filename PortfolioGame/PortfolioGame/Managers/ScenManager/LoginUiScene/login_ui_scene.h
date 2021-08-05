#pragma once
#include "../scene.h"
class MyBitmap;
class LoginUiScene  : public Scene
{
public:
	LoginUiScene();
	~LoginUiScene();
	// Scene을(를) 통해 상속됨
	virtual int ReadyScene() override;
	virtual void UpdateScene() override;
	virtual void LateUpdateScene() override;
	virtual void RenderScene(HDC hdc) override;
	virtual void ReleaseScene() override;

private:

	MyBitmap* _loginUiImage;
};

