#include "../../../pch.h"
#include "mob_gage.h"
#include "../../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../../Components/MapObject/Monster/monster.h"

MobGage::MobGage(): _monster(nullptr)
{
}

MobGage::~MobGage()
{
}

void MobGage::SetMonster(Monster* monster)
{
	_monster = monster;
}

Monster* MobGage::GetMonster() const
{
	return _monster;
}

void MobGage::SetIcon(const std::shared_ptr<MyBitmap>& icon)
{
	_icon = icon;
}

std::shared_ptr<MyBitmap> MobGage::GetIcon() const
{
	return _icon;
}

void MobGage::SetBaseBar(const std::shared_ptr<MyBitmap>& base_bar)
{
	_base_bar = base_bar;
}

std::shared_ptr<MyBitmap> MobGage::GetBaseBar() const
{
	return _base_bar;
}

void MobGage::SetHpBar(const std::shared_ptr<MyBitmap>& hp_bar)
{
	_hp_bar = hp_bar;
}

std::shared_ptr<MyBitmap> MobGage::GetHpBar() const
{
	return _hp_bar;
}

void MobGage::ReadyMobGate()
{
	_icon = std::make_shared<MyBitmap>(MyBitmap());
	_icon->Insert_Bitmap(_hWnd, L"Client\\Ui\\MobGage\\8800000.bmp");
	_base_bar = std::make_shared<MyBitmap>(MyBitmap());
	_base_bar->Insert_Bitmap(_hWnd, L"Client\\Ui\\MobGage\\0.bmp");
	_hp_bar = std::make_shared<MyBitmap>(MyBitmap());
	_hp_bar->Insert_Bitmap(_hWnd, L"Client\\Ui\\MobGage\\1.bmp");
}

void MobGage::UpdateMobGate()
{
	if (_monster != nullptr)
	{
		if (_monster->GetState() == GameObject::State::kDead)
		{
			_monster = nullptr;
		}
	}
}

void MobGage::RenderMobGage(HDC hdc)
{
	if (_monster != nullptr)
	{
		const float hp_percent =
			static_cast<float>(_monster->GetHp()) / static_cast<float>(_monster->GetMaxHp()) * 100.f;
		const int mpWidth = static_cast<int>(_hp_bar->GetWidth() * hp_percent / 100);
		_icon->RenderBitmapImage(hdc, 10, 15, _icon->GetWidth(), _icon->GetHeight());
		_base_bar->RenderBitmapImage(hdc, 44, 23, _base_bar->GetWidth(), _base_bar->GetHeight());
		_hp_bar->RenderBitmapImage(hdc, 46, 25, mpWidth, _hp_bar->GetHeight(), mpWidth, _hp_bar->GetHeight());
	}
}
