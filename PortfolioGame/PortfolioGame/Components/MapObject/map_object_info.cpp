#include "../../pch.h"
#include "map_object_info.h"
#include "../../Managers/ScrollManager/scroll_manager.h"
#include "../../../Common/Managers/BitmapManager/my_bitmap.h"

MapObjectInfo::MapObjectInfo(uint8_t layer) :
	GameObject(layer)
{
}

MapObjectInfo::~MapObjectInfo()
{
}

int MapObjectInfo::ReadyGameObject()
{
	return 0;
}

void MapObjectInfo::UpdateGameObject(const float deltaTime)
{
}

void MapObjectInfo::RenderGameObject(HDC hdc)
{
	if (this->GetImage() != nullptr)
	{
		this->GetImage()->RenderBitmapImage(hdc,
			static_cast<int>(this->GetInfo().x + ScrollManager::GetScrollX() - (this->GetImage()->GetWidth() / 2)),
			static_cast<int>(this->GetInfo().y + ScrollManager::GetScrollY() - (this->GetImage()->GetHeight() / 2)),
			static_cast<int>(this->GetImage()->GetWidth()),
			static_cast<int>(this->GetImage()->GetHeight()));
	}
}

void MapObjectInfo::LateUpdateGameObject()
{
}
