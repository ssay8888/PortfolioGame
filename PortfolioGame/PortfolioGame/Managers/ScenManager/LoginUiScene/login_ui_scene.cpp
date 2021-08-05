#include "../../../pch.h"
#include "login_ui_scene.h"
#include "../../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../KeyManaer/key_manager.h"
#include "../../ScenManager/scene_manager.h"
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
    KeyManager::GetInstance()->KeyUpdate();

    if (KeyManager::GetInstance()->KeyDown(KEY_LBUTTON))
    {
        SceneManager::GetInstance()->SceneChange(SceneManager::SceneState::kInGame);
    }
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
