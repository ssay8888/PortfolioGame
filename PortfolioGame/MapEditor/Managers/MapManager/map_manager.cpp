#include "../../pch.h"
#include "map_manager.h"
#include "map_object.h"
#include "foot_hold.h"
#include "../KeyManaer/key_manager.h"
#include "../ScrollManager/scroll_manager.h"
#include "../../Object/mouse.h"
#include "../../../Common/Utility/file_manager.h"
#include "../../../Common/Utility/string_tools.h"
#include "../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../../Common/Managers/CollisionManager/Collision_Manager.h"

MapManager::MapManager() :
	_selectImage(nullptr),
	_mouse(nullptr)
{
}

void MapManager::Ready_Map()
{

	auto fileNams = FileManager::GetInstance()->GetDirFileName(L"Client\\Map\\Tile\\woodMarble.img\\");

	std::map<std::string, MyBitmap*> list;
	for (auto begin : fileNams)
	{
		MyBitmap* image = new MyBitmap;
		image->Insert_Bitmap(_hWnd, begin.c_str());
		list.insert({ FileManager::GetInstance()->GetFileName(StringTools::WStringToString(begin.c_str())), image });
		std::cout << begin.c_str() << std::endl;
	}
	_images.insert(std::make_pair("Client\\Map\\Tile\\woodMarble.img\\", list));

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
	if (KeyManager::GetInstance()->KeyDown(KEY_LBUTTON))
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
	else if (KeyManager::GetInstance()->KeyDown(KEY_RBUTTON))
	{
		auto foot = new FootHold();
		foot->SetStartPos(pt.x, pt.y);
		_footholds.emplace_back(foot);
	}
	else if (KeyManager::GetInstance()->KeyPressing(KEY_RBUTTON))
	{
		_footholds.back()->SetEndPos(pt.x, pt.y);
	}
	else if (KeyManager::GetInstance()->KeyUp(KEY_RBUTTON))
	{
		_footholds.back()->SetEndPos(pt.x, pt.y);
	}
	else if (KeyManager::GetInstance()->KeyUp(KEY_S))
	{
		SaveData();
	}
	else if (KeyManager::GetInstance()->KeyUp(KEY_L))
	{
		LoadData();
	}
	else if (KeyManager::GetInstance()->KeyUp(KEY_Z))
	{
		_list[SelectLayer].pop_back();
	}

}

void MapManager::Render_Map(HDC hDC)
{
	POINT pt{};
	GetCursorPos(&pt);
	ScreenToClient(_hWnd, &pt);

	//Gdiplus::Graphics Mouse(hDC);
	if (_selectImage != nullptr)
	{
		/*Gdiplus::Rect rc{
			static_cast<int>(GetMouse()->GetInfo().x - (_selectImage->GetWidth() / 2)),
			static_cast<int>(GetMouse()->GetInfo().y - (_selectImage->GetHeight() / 2)),
			static_cast<int>(_selectImage->GetWidth()),
			static_cast<int>(_selectImage->GetHeight())
		};*/

		_selectImage->RenderBitmapImage(hDC, 
			static_cast<int>(GetMouse()->GetInfo().x - (_selectImage->GetWidth() / 2)),
			static_cast<int>(GetMouse()->GetInfo().y - (_selectImage->GetHeight() / 2)),
			static_cast<int>(_selectImage->GetWidth()),
			static_cast<int>(_selectImage->GetHeight()));
		//GdiTransparentBlt(hDC,
		//	static_cast<int>(GetMouse()->GetInfo().x - (_selectImage->GetWidth() / 2)),
		//	static_cast<int>(GetMouse()->GetInfo().y - (_selectImage->GetHeight() / 2)),
		//	static_cast<int>(_selectImage->GetWidth()),
		//	static_cast<int>(_selectImage->GetHeight()),
		//	_selectImage->GetMemDC(),
		//	0, 0,
		//	static_cast<int>(_selectImage->GetWidth()),
		//	static_cast<int>(_selectImage->GetHeight()),
		//	RGB(255, 0, 255));
		//Mouse.DrawImage(_selectImage, rc);
	}
	int countX = 0;
	int countY = 0;
	for (auto tiles : _images)
	{
		for (auto tile : tiles.second)
		{
			
			//Gdiplus::Graphics destination(hDC);

			/*RECT rc {
				static_cast < int>(90 + (90 * countX) + 800 ),
				static_cast < int>(70 + (70 * countY)),
				static_cast<int>(tile.second->GetWidth()),
				static_cast<int>(tile.second->GetHeight())
			};*/

			//Rectangle(hDC, rc.left, rc.top, rc.right, rc.bottom);

			tile.second->RenderBitmapImage(hDC,
				static_cast <int>(90 + (90 * countX) + 800),
				static_cast <int>(70 + (70 * countY)),
				static_cast<int>(tile.second->GetWidth()),
				static_cast<int>(tile.second->GetHeight()));
			//destination.DrawImage(tile.second, rc);

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
			/*Gdiplus::Graphics destination(hDC);

			Gdiplus::Rect rc{
				static_cast<int>(map->GetInfo().x + static_cast<int>(ScrollManager::GetScrollX() - (map->GetImage()->GetWidth() / 2))),
				static_cast<int>(map->GetInfo().y + static_cast<int>(ScrollManager::GetScrollY() - (map->GetImage()->GetHeight() / 2))),
				static_cast<int>(map->GetImage()->GetWidth()),
				static_cast<int>(map->GetImage()->GetHeight())
			};*/

			map->GetImage()->RenderBitmapImage(hDC,
				static_cast<int>(map->GetInfo().x + static_cast<int>(ScrollManager::GetScrollX() - (map->GetImage()->GetWidth() / 2))),
				static_cast<int>(map->GetInfo().y + static_cast<int>(ScrollManager::GetScrollY() - (map->GetImage()->GetHeight() / 2))),
				static_cast<int>(map->GetImage()->GetWidth()),
				static_cast<int>(map->GetImage()->GetHeight()));
			//destination.DrawImage(map->GetImage(), rc);
		}
	}

	for (auto foot : _footholds)
	{
		foot->RenderFootHold(hDC);
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
		obj->SetFileName(_selectFileName);
		obj->SetPath(_selectPath);
		obj->SetPos(
			static_cast<float>(GetMouse()->GetInfo().x),
			static_cast<float>(GetMouse()->GetInfo().y));
		obj->SetCX(_selectImage->GetWidth());
		obj->SetCY(_selectImage->GetHeight());

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
				_selectFileName = tile.first;
				_selectPath = tiles.first;
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
	HANDLE hFile = CreateFile(L"../Data/Map.dat",
		GENERIC_WRITE,
		0, nullptr,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, 
		nullptr); 

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(nullptr, L"저장실패", L"확인", MB_OK);

		return;
	}
	unsigned short mark = 0xFEFF;
	DWORD dwByte = 0;
	WriteFile(hFile, &mark, sizeof(mark), &dwByte, nullptr);
	for (auto objs : _list)
	{
		
		size_t size = objs.size();
		WriteFile(hFile, &size, sizeof(size), &dwByte, nullptr);
		for (auto obj : objs)
		{

			size_t pathsize = strlen(obj->GetPath().c_str()) * sizeof(wchar_t);
			WriteFile(hFile, &pathsize, sizeof(pathsize), &dwByte, nullptr);
			WriteFile(hFile, obj->GetPath().c_str(), static_cast<DWORD>(pathsize), &dwByte, nullptr);

			size_t filenamesize = strlen(obj->GetFileName().c_str()) * sizeof(wchar_t);
			WriteFile(hFile, &filenamesize, sizeof(filenamesize), &dwByte, nullptr);
			WriteFile(hFile, obj->GetFileName().c_str(), static_cast<DWORD>(filenamesize), &dwByte, nullptr);
			auto info = obj->GetInfo();
			WriteFile(hFile, &info, sizeof(obj->GetInfo()), &dwByte, nullptr);
			auto rect = obj->GetRect();
			WriteFile(hFile, &rect, sizeof(obj->GetRect()), &dwByte, nullptr);
			uint32_t layer = obj->GetLayer();
			WriteFile(hFile, &layer, sizeof(obj->GetLayer()), &dwByte, nullptr);
		}
	}
	size_t footSize = _footholds.size();
	WriteFile(hFile, &footSize, sizeof(footSize), &dwByte, nullptr);
	for (auto foot : _footholds) 
	{
		auto startPos = foot->GetStartPos();
		WriteFile(hFile, &startPos, sizeof(startPos), &dwByte, nullptr);
		auto endPos = foot->GetEndPos();
		WriteFile(hFile, &endPos, sizeof(endPos), &dwByte, nullptr);
	}

	CloseHandle(hFile);
	MessageBox(nullptr, L"저장완료", L"확인", MB_OK);
}

void MapManager::LoadData()
{
	HANDLE hFile = CreateFile(L"../Data/Map.dat",
		GENERIC_READ,
		0, nullptr,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, 
		nullptr); 

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(nullptr, L"로드실패", L"확인", MB_OK);

		return;
	}

	DWORD dwByte = 0;

	unsigned short mark = 0xFEFF;
	bool  check = ReadFile(hFile, &mark, sizeof(mark), &dwByte, nullptr);
	for (int i = 0; i < 7; i++)
	{
		size_t size;
		check = ReadFile(hFile, &size, sizeof(size), &dwByte, nullptr);
		for (int i =0; i < size; i++)
		{
			MapObject* obj = new MapObject();
			size_t pathSize;
			check = ReadFile(hFile, &pathSize, sizeof(pathSize), &dwByte, nullptr);
			char* path = new char[pathSize];
			ZeroMemory(path, pathSize);
			check = ReadFile(hFile, path, static_cast<DWORD>(pathSize), &dwByte, nullptr);
			obj->SetPath(path);
			delete[] path;

			size_t fileSize;
			check = ReadFile(hFile, &fileSize, sizeof(fileSize), &dwByte, nullptr);
			char* fileName = new char[fileSize];
			ZeroMemory(fileName, fileSize);
			check = ReadFile(hFile, fileName, static_cast<DWORD>(fileSize), &dwByte, nullptr);
			obj->SetFileName(fileName);
			delete[] fileName;

			Info info;
			check = ReadFile(hFile, &info, sizeof(info), &dwByte, nullptr);
			obj->SetInfo(info);
			RECT rect;
			check = ReadFile(hFile, &rect, sizeof(rect), &dwByte, nullptr);
			obj->SetRect(rect);
			uint32_t layer;
			check = ReadFile(hFile, &layer, sizeof(layer), &dwByte, nullptr);
			obj->SetLayer(layer);
			
			std::string fullPath;
			fullPath.append(obj->GetPath()).append(obj->GetFileName());
			auto image = _images.find(obj->GetPath());
			auto file = image->second.find(obj->GetFileName());
			obj->SetImage(file->second);
			_list[layer].push_back(obj);
		}
	}

	size_t footSize;
	check = ReadFile(hFile, &footSize, sizeof(footSize), &dwByte, nullptr);
	for (int i = 0; i < footSize; i++)
	{
		FootHold* footHold = new FootHold();
		POINT startPos;
		POINT endPos;
		check = ReadFile(hFile, &startPos, sizeof(startPos), &dwByte, nullptr);
		check = ReadFile(hFile, &endPos, sizeof(endPos), &dwByte, nullptr);
		footHold->SetStartPos(startPos.x, startPos.y);
		footHold->SetEndPos(endPos.x, endPos.y);
		_footholds.push_back(footHold);
	}

	CloseHandle(hFile);
	MessageBox(nullptr, L"로드 성공", L"확인", MB_OK);
}

Mouse* MapManager::GetMouse()
{
	return _mouse;
}
