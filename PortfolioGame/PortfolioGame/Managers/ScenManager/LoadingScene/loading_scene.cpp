#include "../../../pch.h"
#include "loading_scene.h"
#include "../../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../KeyManaer/key_manager.h"
#include "../../ScenManager/scene_manager.h"

LoadingScene::LoadingScene() :
	_logoFrameCount(0),
	_frameDelay(75),
	_WindowCX(800),
	_WindowCY(600),
	_tick(0)
{
	ReadyScene();
}

LoadingScene::~LoadingScene()
{
	ReleaseScene();
}

int LoadingScene::ReadyScene()
{
	const int size = 256;
	wchar_t path[size];
	for (int i = 0; i < 60; i++)
	{
		ZeroMemory(path, size);
		MyBitmap* bitmap = new MyBitmap();
		swprintf_s(path, size, L"Client\\Ui\\Logo.img\\Wizet.%d.bmp", i);
		bitmap->Insert_Bitmap(_hWnd, path);
		_logoImages.push_back(bitmap);
	}

	MyBitmap* lastLogo = new MyBitmap();
	lastLogo->Insert_Bitmap(_hWnd, L"Client\\Ui\\Logo.img\\Nexon.0.bmp");
	_lastLogo = lastLogo;

	RECT rc{ 0, 0, static_cast<LONG>(_WindowCX), static_cast<LONG>(_WindowCY) };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	SetWindowPos(_hWnd, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, 0);
	return 0;
}

void LoadingScene::UpdateScene()
{
	KeyManager::GetInstance()->KeyUpdate();
	if (KeyManager::GetInstance()->KeyDown(KEY_LBUTTON))
	{
		if (_logoFrameCount < _logoImages.size() - 10)
		{
			_logoFrameCount = static_cast<int>(_logoImages.size() - 10);
		}
	}
}

void LoadingScene::RenderScene(HDC hdc)
{
	if (_logoFrameCount >= 70)
	{
		SceneManager::GetInstance()->SceneChange(SceneManager::SceneState::kLogin);
		return;
	}
	if (_logoFrameCount >= _logoImages.size() )
	{
		_lastLogo->RenderBitmapImage(hdc, 0, 0, _WindowCX, _WindowCY);
	}
	else if (_logoFrameCount >= 0)
	{
		_logoImages[_logoFrameCount]->RenderBitmapImage(hdc, 0, 0, _WindowCX, _WindowCY);
	}
}

void LoadingScene::ReleaseScene()
{
	for (auto data : _logoImages)
	{
		delete data;
		data = nullptr;
	}
	delete _lastLogo;
}

void LoadingScene::LateUpdateScene()
{
	ULONGLONG nowTick = GetTickCount64();
	if (nowTick > _tick + _frameDelay)
	{
		++_logoFrameCount;
		_tick = nowTick;
	}
}
