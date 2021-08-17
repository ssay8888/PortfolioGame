#include "../../pch.h"
#include "ani_map_object.h"
#include "../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../Managers/ScrollManager/scroll_manager.h"

AniMapObject::AniMapObject() :
	GameObject(5),
	_frame_number(0),
	_frame_tick(0)
{
}

AniMapObject::~AniMapObject()
{
}

int AniMapObject::ReadyGameObject()
{
	return 0;
}

void AniMapObject::UpdateGameObject(const float deltaTime)
{
}

void AniMapObject::RenderGameObject(HDC hdc)
{
	auto data = _ani_image[_frame_number % _ani_image.size()];
	data.second->RenderBitmapImage(hdc,
		static_cast<int>(_rect.left  + ScrollManager::GetScrollX()),
		static_cast<int>(_rect.bottom - data.first.y + ScrollManager::GetScrollY()),
		data.second->GetWidth(),
		data.second->GetHeight(),
		data.second->GetWidth(),
		data.second->GetHeight(),
		RGB(0, 0, 0)); 
	//Rectangle(hdc, 
	//	static_cast<int>(_rect.left + ScrollManager::GetScrollX()),
	//	static_cast<int>(_rect.top + ScrollManager::GetScrollY()),
	//	static_cast<int>(_rect.right + ScrollManager::GetScrollX()),
	//	static_cast<int>(_rect.bottom + ScrollManager::GetScrollY()));
	if (GetTickCount64() > _frame_tick + 100)
	{
		_frame_number++;
		_frame_tick = GetTickCount64();
	}
}

void AniMapObject::LateUpdateGameObject()
{
}

void AniMapObject::InsertAni(ObjectPos pos, MyBitmap* image)
{
	_ani_image.push_back(std::make_pair(pos, image));
}
