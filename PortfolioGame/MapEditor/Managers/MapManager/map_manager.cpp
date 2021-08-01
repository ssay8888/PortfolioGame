#include "../../pch.h"
#include "map_manager.h"
#include "../KeyManaer/key_manager.h"
#include "../ScrollManager/scroll_manager.h"
#include "../../../Common/Utility/file_manager.h"
#include "../../../Common/Managers/CollisionManager/Collision_Manager.h"
#include "map_object.h"
#include "../../Object/mouse.h"

MapManager::MapManager() :
	_selectImage(nullptr),
	_mouse(nullptr)
{
}

void MapManager::Ready_Map()
{
	auto asd = FileManager::GetInstance()->GetDirFileName(L"Client\\Map\\Tile\\woodMarble.img\\");

	std::list<Gdiplus::Image*> list;
	for (auto begin : asd) {
		auto image = new Gdiplus::Image(begin.c_str());
		list.push_back(image);
		std::cout << begin.c_str() << std::endl;
	}
	_images.insert(std::make_pair(L"woodMarble.img", list));

	_mouse = new Mouse();
}

void MapManager::Update_Map()
{
	POINT pt{};
	GetCursorPos(&pt);
	ScreenToClient(_hWnd, &pt);
	GetMouse()->SetPos(static_cast<float>(pt.x), static_cast<float>(pt.y));
	if (_selectImage != nullptr)
	{
		MouseUpdate(pt);
	}
	if (KeyManager::Get_Instance()->KeyDown(KEY_LBUTTON))
	{
		if (pt.x < 800 && pt.y < 600)
		{
			AddListObject();
		}
		else
		{
			SelectImage(pt);
		}
	}
	else if (KeyManager::Get_Instance()->KeyDown(KEY_RBUTTON))
	{

	}

}

void MapManager::Render_Map(HDC hDC)
{
	POINT pt{};
	GetCursorPos(&pt);
	ScreenToClient(_hWnd, &pt);

	Gdiplus::Graphics Mouse(hDC);
	if (_selectImage != nullptr)
	{
		Gdiplus::Rect rc{
			static_cast<int>(GetMouse()->GetInfo().x - (_selectImage->GetWidth() / 2)),
			static_cast<int>(GetMouse()->GetInfo().y - (_selectImage->GetHeight() / 2)),
			static_cast<int>(_selectImage->GetWidth()),
			static_cast<int>(_selectImage->GetHeight())
		};
		Mouse.DrawImage(_selectImage, rc);
	}
	int countX = 0;
	int countY = 0;
	for (auto tiles : _images)
	{
		for (auto tile : tiles.second)
		{
			Gdiplus::Graphics destination(hDC);

			Gdiplus::Rect rc {
				static_cast < int>(90 + (90 * countX) + 800 ),
				static_cast < int>(70 + (70 * countY)),
				static_cast<int>(tile->GetWidth()),
				static_cast<int>(tile->GetHeight())
			};
			Rectangle(hDC, rc.GetLeft(), rc.GetTop(), rc.GetRight(), rc.GetBottom());
			destination.DrawImage(tile, rc);

			if (countX++ > 0 && countX % 4 == 0)
			{
				countX = 0;
				countY += 1;
			}
		}
	}

	for (auto& maps : _list)
	{
		for (auto& map : maps)
		{
			Gdiplus::Graphics destination(hDC);

			Gdiplus::Rect rc{
				static_cast<int>(map->GetInfo().x + static_cast<int>(ScrollManager::GetScrollX() - (map->GetImage()->GetWidth() / 2))),
				static_cast<int>(map->GetInfo().y + static_cast<int>(ScrollManager::GetScrollY() - (map->GetImage()->GetHeight() / 2))),
				static_cast<int>(map->GetImage()->GetWidth()),
				static_cast<int>(map->GetImage()->GetHeight())
			};
			destination.DrawImage(map->GetImage(), rc);
		}
	}

	std::wstring str;
	for (int i = 0; i < 7; i++)
	{
		str.append(std::to_wstring(_list[i].size())).append(L"°³");
	}
	TextOut(hDC, 0, 0, str.c_str(), static_cast<int>(str.size()));
}

void MapManager::AddListObject()
{
	if (_selectImage != nullptr)
	{
		MapObject* obj = new MapObject();
		obj->SetLayer(SelectLayer);
		obj->SetImage(_selectImage);
		obj->SetPos(
			static_cast<float>(GetMouse()->GetInfo().x),
			static_cast<float>(GetMouse()->GetInfo().y));

		_list[obj->GetLayer()].emplace_back(obj);
	}
}

void MapManager::SelectImage(POINT& pt)
{
	int countX = 0;
	int countY = 0;
	for (auto tiles : _images)
	{
		for (auto tile : tiles.second)
		{
			RECT rc{};
			int x = 90 + (90 * countX) + 800;
			int y = 70 + (70 * countY);
			RECT tile_rc{ x, y, x + static_cast<LONG>(tile->GetWidth()), y + static_cast<LONG>(tile->GetHeight()) };
			RECT mouse_rc{ pt.x, pt.y, pt.x + 1, pt.y + 1 };
			if (IntersectRect(&rc, &mouse_rc, &tile_rc))
			{
				_selectImage = tile;
			}
			if (countX++ > 0 && countX % 4 == 0)
			{
				countX = 0;
				countY += 1;
			}
		}
	}
}

void MapManager::MouseUpdate(POINT& pt)
{
	GetMouse()->SetCX(_selectImage->GetWidth());
	GetMouse()->SetCY(_selectImage->GetHeight());
	GetMouse()->UpdateRect();
	int countX = 0;
	int countY = 0;
	for (auto tiles : _images)
	{
		for (auto tile : tiles.second)
		{
			RECT rc{};
			int x = 90 + (90 * countX) + 800;
			int y = 70 + (70 * countY);
			for (auto objs : _list)
			{
				for (auto obj : objs)
				{
					obj->UpdateRect();
					CollisionManager::Collision_RectEX<MapObject>(obj, GetMouse());
				}
			}

			if (countX++ > 0 && countX % 4 == 0)
			{
				countX = 0;
				countY += 1;
			}
		}
	}
}

Mouse* MapManager::GetMouse()
{
	return _mouse;
}
