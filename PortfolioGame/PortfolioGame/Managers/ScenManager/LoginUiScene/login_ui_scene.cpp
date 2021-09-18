#include "../../../pch.h"
#include "login_ui_scene.h"
#include "../../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../../Network/client_session.h"
#include "../../KeyManaer/key_manager.h"
#include "../../ScenManager/scene_manager.h"
#include "../../SoundManager/sound_manager.h"
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

    SoundManager::GetInstance()->PlayBGM(L"Title.mp3");
    return 0;
}

void LoginUiScene::UpdateScene()
{
    POINT mouse;
    GetCursorPos(&mouse);
    ScreenToClient(_hWnd, &mouse);

    KeyManager::GetInstance()->KeyUpdate();

    if (KeyManager::GetInstance()->KeyDown(KEY_LBUTTON))
    {
        RECT rc1{ 454, 282, 504, 328 };
        RECT rc2{ 100, 150, 150, 200 };
        if (PtInRect(&rc1, mouse))
        {
           // SceneManager::GetInstance()->SceneChange(SceneManager::SceneState::kInGame);
            OutPacket* out_packet = new OutPacket();
            out_packet->Encode1(static_cast<char>(::opcode::ClientOpcode::kLoginRequest));
            out_packet->Encode1(0);
            _client_session->SendPacket(out_packet);
        } else if (PtInRect(&rc2, mouse))
        {
            //SceneManager::GetInstance()->SceneChange(SceneManager::SceneState::kInGame);
            OutPacket* out_packet = new OutPacket();
            out_packet->Encode1(static_cast<char>(::opcode::ClientOpcode::kLoginRequest));
            out_packet->Encode1(1);
            _client_session->SendPacket(out_packet);
        }
    }
}

void LoginUiScene::LateUpdateScene()
{
}

void LoginUiScene::RenderScene(HDC hdc)
{
    _loginUiImage->RenderBitmapImage(hdc, 0, 0, _loginUiImage->GetWidth(), _loginUiImage->GetHeight());
    //Rectangle(hdc, 100, 100, 150, 150);
    //Rectangle(hdc, 454, 282, 504, 328);
}

void LoginUiScene::ReleaseScene()
{
}
