#include "../../pch.h"
#include "map_manager.h"
#include "map_object.h"
#include "foot_hold.h"
#include "../KeyManaer/key_manager.h"
#include "../ScrollManager/scroll_manager.h"
#include "../../Object/mouse.h"
#include "../../../Common/Utility/file_manager.h"
#include "../../../Common/Managers/CollisionManager/Collision_Manager.h"

MapManager::MapManager() :
	_selectImage(nullptr),
	_mouse(nullptr)
{
}

void MapManager::Ready_Map()
{
	auto asd = FileManager::GetInstance()->GetDirFileName(L"Client\\Map\\Tile\\woodMarble.img\\");

	std::map<std::wstring, Gdiplus::Image*> list;
	for (auto begin : asd) {
		auto image = new Gdiplus::Image(begin.c_str());
		list.insert({ begin.c_str(), image });
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
		auto foot = new FootHold();
		foot->SetStartPos(pt.x, pt.y);
		_foot[SelectLayer].emplace_back(foot);
	}
	else if (KeyManager::Get_Instance()->KeyPressing(KEY_RBUTTON))
	{
		_foot[SelectLayer].back()->SetEndPos(pt.x, pt.y);
	}
	else if (KeyManager::Get_Instance()->KeyUp(KEY_RBUTTON))
	{
		_foot[SelectLayer].back()->SetEndPos(pt.x, pt.y);
	}
	else if (KeyManager::Get_Instance()->KeyUp(KEY_S))
	{
		SaveData();
	}
	else if (KeyManager::Get_Instance()->KeyUp(KEY_L))
	{
		LoadData();
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
				static_cast<int>(tile.second->GetWidth()),
				static_cast<int>(tile.second->GetHeight())
			};
			Rectangle(hDC, rc.GetLeft(), rc.GetTop(), rc.GetRight(), rc.GetBottom());
			destination.DrawImage(tile.second, rc);

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

	for (auto foots : _foot)
	{ 
		for (auto foot : foots)
		{
			foot->RenderFootHold(hDC);
		}
	}

	std::wstring str;
	for (int i = 0; i < 7; i++)
	{
		str.append(std::to_wstring(_list[i].size())).append(L"개");
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
		obj->SetPath(_selectPath);
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
			RECT tile_rc{ x, y, x + static_cast<LONG>(tile.second->GetWidth()), y + static_cast<LONG>(tile.second->GetHeight()) };
			RECT mouse_rc{ pt.x, pt.y, pt.x + 1, pt.y + 1 };
			if (IntersectRect(&rc, &mouse_rc, &tile_rc))
			{
				_selectImage = tile.second;
				_selectPath = tile.first;
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

void MapManager::SaveData()
{
	HANDLE hFile = CreateFile(L"../Data/Map.dat",// 경로
		GENERIC_WRITE,//쓸것인지 읽을 것인지 
		0, nullptr,// 파일 공유 및 보안 속성이건 건드릴 필요 없음. 
		CREATE_ALWAYS,// 늘 새로 만들겠다. 
		FILE_ATTRIBUTE_NORMAL,//애는 파일 만들때 속성 어떻게 할건지. 그냥 보통. 건드릴 필요 없음. 
		nullptr);// 이건 그냥 널널 

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(nullptr, L"저장실패", L"확인", MB_OK);

		return;
	}

	DWORD dwByte = 0;
	for (auto objs : _list)
	{
		
		size_t size = objs.size();
		WriteFile(hFile, &size, sizeof(size), &dwByte, nullptr);
		for (auto obj : objs)
		{
			size_t pathsize = obj->GetPath().size();
			WriteFile(hFile, &pathsize, sizeof(pathsize), &dwByte, nullptr);

			WriteFile(hFile, obj->GetPath().c_str(), static_cast<DWORD>(pathsize), &dwByte, nullptr);
			auto info = obj->GetInfo();
			WriteFile(hFile, &info, sizeof(obj->GetInfo()), &dwByte, nullptr);
			auto rect = obj->GetRect();
			WriteFile(hFile, &rect, sizeof(obj->GetRect()), &dwByte, nullptr);
			uint32_t layer = obj->GetLayer();
			WriteFile(hFile, &layer, sizeof(obj->GetLayer()), &dwByte, nullptr);
			//WriteFile(hFile, obj->Get_LineInfo(), sizeof(LINEINFO), &dwByte, nullptr);
		}
	}

	CloseHandle(hFile);
	MessageBox(nullptr, L"저장완료", L"확인", MB_OK);
}

void MapManager::LoadData()
{
	HANDLE hFile = CreateFile(L"../Data/Map.dat",// 경로
		GENERIC_READ,//쓸것인지 읽을 것인지 
		0, nullptr,// 파일 공유 및 보안 속성이건 건드릴 필요 없음. 
		OPEN_EXISTING,// 늘 새로 만들겠다. 
		FILE_ATTRIBUTE_NORMAL,//애는 파일 만들때 속성 어떻게 할건지. 그냥 보통. 건드릴 필요 없음. 
		nullptr);// 이건 그냥 널널 

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(nullptr, L"나랏말쌈이뒹귁에닳아 사맛디아니할세 살어리 살어리 랏다 청산에 살어리 랏다 이런들 어떠하리 저런들 어떠하리 이몸이 죽고 죽어 일백번 고쳐죽어", L"공부좀하자 애들아...", MB_OK);

		return;
	}

	DWORD dwByte = 0;

	for (int i = 0; i < 7; i++)
	{
		size_t size;
		ReadFile(hFile, &size, sizeof(size), &dwByte, nullptr);
		for (int i =0; i < size; i++)
		{
			MapObject* obj = new MapObject();
			size_t pathSize;
			ReadFile(hFile, &pathSize, sizeof(pathSize), &dwByte, nullptr);
			WCHAR* path = new WCHAR[pathSize * 2];
			ZeroMemory(path, pathSize * 2);
			ReadFile(hFile, path, static_cast<DWORD>(pathSize), &dwByte, nullptr);
			obj->SetPath(path);
			Info info;
			ReadFile(hFile, &info, sizeof(info), &dwByte, nullptr);
			RECT rect;
			ReadFile(hFile, &rect, sizeof(rect), &dwByte, nullptr);
			uint32_t layer;
			ReadFile(hFile, &layer, sizeof(layer), &dwByte, nullptr);
			
			_list[layer].push_back(obj);
		}
	}



	CloseHandle(hFile);
	MessageBox(nullptr, L"성공!", L"성공성공!!!", MB_OK);
}

Mouse* MapManager::GetMouse()
{
	return _mouse;
}
