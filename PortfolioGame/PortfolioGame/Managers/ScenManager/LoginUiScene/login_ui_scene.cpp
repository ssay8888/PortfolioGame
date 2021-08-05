#include "../../../pch.h"
#include "login_ui_scene.h"
#include "../../../../Common/Managers/BitmapManager/my_bitmap.h"
LoginUiScene::LoginUiScene():
    _loginUiImage(nullptr)
{
    ReadyScene();
}

LoginUiScene::~LoginUiScene()
{
}

int LoginUiScene::ReadyScene()
{
    _loginUiImage = new MyBitmap();
    _loginUiImage->Insert_Bitmap(_hWnd, L"Client\\Ui\\Login.bmp");

    return 0;
}

void LoginUiScene::UpdateScene()
{
}

void LoginUiScene::LateUpdateScene()
{
}

void LoginUiScene::RenderScene(HDC hdc)
{
    _loginUiImage->RenderBitmapImage(hdc, 0, 0, _loginUiImage->GetWidth(), _loginUiImage->GetHeight());
}

void LoginUiScene::ReleaseScene()
{
}
