#include "../../../pch.h"
#include "npc.h"

#include "../../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../../Managers/ScrollManager/scroll_manager.h"

Npc::Npc() : GameObject(0),
             _npc_id(0),
             _origin(),
             _frame_tick(0),
             _frame_number(0)
{
}

Npc::~Npc()
{
}

void Npc::InsertImage(std::shared_ptr<MyBitmap> image)
{
	_stand.emplace_back(image);
}

std::vector<std::shared_ptr<MyBitmap>>& Npc::GetStandImages()
{
	return _stand;
}

void Npc::SetOrigin(POINT point)
{
	_origin = point;
}

void Npc::SetOriginX(int x)
{
	_origin.x = x;
}

void Npc::SetOriginY(int y)
{
	_origin.y = y;
}

void Npc::SetNpcId(int32_t npc_id)
{
	_npc_id = npc_id;
}

void Npc::SetNpcPath(std::string path)
{
	_npc_path = path;
}

std::string Npc::GetNpcPath() const
{
	return _npc_path;
}

POINT Npc::GetOrigin() const
{
	return _origin;
}

int32_t Npc::GetNpcId() const
{
	return _npc_id;
}

int Npc::ReadyGameObject()
{
	return 0;
}

void Npc::UpdateGameObject(const float deltaTime)
{
	if (!_stand.empty())
	{
		auto image = _stand[_frame_number % _stand.size()];
		if (image != nullptr)
		{
			_info.cx = image->GetWidth();
			_info.cy = image->GetHeight();
		}
	}
	
}

void Npc::RenderGameObject(HDC hdc)
{
	UpdateRectGameObject();
	if (!_stand.empty())
	{
		//Rectangle(hdc,
		//	_rect.left + static_cast<int>(ScrollManager::GetScrollX()),
		//	_rect.top + static_cast<int>(ScrollManager::GetScrollY()),
		//	_rect.right + static_cast<int>(ScrollManager::GetScrollX()),
		//	_rect.bottom + static_cast<int>(ScrollManager::GetScrollY()));
		_stand[_frame_number % _stand.size()]->RenderBitmapImage(hdc, 
			static_cast<int>(_info.x - (_info.cx >> 1) + ScrollManager::GetScrollX()), 
			static_cast<int>(_info.y - (_info.cy * 0.5) + ScrollManager::GetScrollY()),
			_info.cx,
			_info.cy);
	}
}

void Npc::LateUpdateGameObject()
{
	if (GetTickCount64() > _frame_tick + 300)
	{
		_frame_tick = GetTickCount64();
	}
}
