#include "../../pch.h"
#include <io.h>
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
	_mouse(nullptr),
	_selectCount(0),
	_mapSize({1024, 768})
{
}

void MapManager::Ready_Map()
{

	TileLoad(L"woodMarble.img");

	_mouse = new Mouse();
}

void MapManager::Update_Map()
{
	POINT pt{};
	GetCursorPos(&pt);
	ScreenToClient(_hWnd, &pt);
	GetMouse()->SetPos(static_cast<float>(pt.x), static_cast<float>(pt.y));
	if (!_selectImage.empty())
	{
		MouseUpdate(pt);
	}
	const int moveValue = 10;
	if (KeyManager::GetInstance()->KeyPressing(KEY_LEFT))
	{
		ScrollManager::GainScrollX(moveValue);
	}
	if (KeyManager::GetInstance()->KeyPressing(KEY_RIGHT))
	{
		ScrollManager::GainScrollX(-moveValue);
	}
	if (KeyManager::GetInstance()->KeyPressing(KEY_UP))
	{
		ScrollManager::GainScrollY(moveValue);
	}
	if (KeyManager::GetInstance()->KeyPressing(KEY_DOWN))
	{
		ScrollManager::GainScrollY(-moveValue);
	}
	if (KeyManager::GetInstance()->KeyDown(KEY_LBUTTON))
	{
		if (pt.x < 1024 && pt.y < 768)
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
		foot->SetStartPos(pt.x - static_cast<int>(ScrollManager::GetScrollX()), pt.y- static_cast<int>(ScrollManager::GetScrollY()));
		_footholds.emplace_back(foot);
	}
	else if (KeyManager::GetInstance()->KeyPressing(KEY_RBUTTON))
	{
		_footholds.back()->SetEndPos(pt.x - static_cast<int>(ScrollManager::GetScrollX()), pt.y - static_cast<int>(ScrollManager::GetScrollY()));
	}
	else if (KeyManager::GetInstance()->KeyUp(KEY_RBUTTON))
	{
		_footholds.back()->SetEndPos(pt.x - static_cast<int>(ScrollManager::GetScrollX()), pt.y - static_cast<int>(ScrollManager::GetScrollY()));
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
		if (!_list[SelectLayer].empty()) {
			_list[SelectLayer].pop_back();
		}
	}

}

void MapManager::Render_Map(HDC hDC)
{
	POINT pt{};
	GetCursorPos(&pt);
	ScreenToClient(_hWnd, &pt);

	int countX = 0;
	int countY = 0;

	Rectangle(hDC, 
		0 + static_cast<int>(ScrollManager::GetScrollX()), 
		0 + static_cast<int>(ScrollManager::GetScrollY()), 
		_mapSize.x + static_cast<int>(ScrollManager::GetScrollX()), 
		_mapSize.y + static_cast<int>(ScrollManager::GetScrollY()));
	for (auto& maps : _list)
	{
		for (auto& map : maps)
		{
			map->GetImage()->RenderBitmapImage(hDC,
				static_cast<int>(map->GetInfo().x + static_cast<int>(ScrollManager::GetScrollX() - (map->GetImage()->GetWidth() / 2))),
				static_cast<int>(map->GetInfo().y + static_cast<int>(ScrollManager::GetScrollY() - (map->GetImage()->GetHeight() / 2))),
				static_cast<int>(map->GetImage()->GetWidth()),
				static_cast<int>(map->GetImage()->GetHeight()));
		}
	}




	for (auto foot : _footholds)
	{
		foot->RenderFootHold(hDC);
	}
	Rectangle(hDC, 1024, 0, 1600, 900);

	if (!_selectImage.empty())
	{
		_selectImage[_selectCount]->RenderBitmapImage(hDC,
			static_cast<int>(GetMouse()->GetInfo().x - (_selectImage[_selectCount]->GetWidth() / 2)),
			static_cast<int>(GetMouse()->GetInfo().y - (_selectImage[_selectCount]->GetHeight() / 2)),
			static_cast<int>(_selectImage[_selectCount]->GetWidth()),
			static_cast<int>(_selectImage[_selectCount]->GetHeight()));
	}

	for (auto tiles : _images)
	{
		for (auto tile : tiles.second)
		{
			tile.second[0]->RenderBitmapImage(hDC,
				static_cast <int>(90 + (90 * countX) + 1024),
				static_cast <int>(70 + (70 * countY)),
				static_cast<int>(tile.second[0]->GetWidth()),
				static_cast<int>(tile.second[0]->GetHeight()));
			if (countX++ > 0 && countX % 4 == 0)
			{
				countX = 0;
				countY += 1;
			}
		}
	}
	std::wstring str;
	for (int i = 0; i < 7; i++)
	{
		str.append(std::to_wstring(_list[i].size())).append(L"개");
	}
	TextOut(hDC, 0, 0, str.c_str(), static_cast<int>(str.size()));
	str.clear();
	str.append(L"X : ").append(std::to_wstring(pt.x - ScrollManager::GetScrollX())).
		append(L"Y : ").append(std::to_wstring(pt.y - ScrollManager::GetScrollY()));
	TextOut(hDC, 0, 20, str.c_str(), static_cast<int>(str.size()));
}

void MapManager::AddListObject()
{
	if (!_selectImage.empty())
	{
		MapObject* obj = new MapObject();
		obj->SetLayer(SelectLayer);
		obj->SetImage(_selectImage[_selectCount]);
		obj->SetImageNumber(_selectCount);
		obj->SetFileName(_selectFileName);
		obj->SetPath(_selectPath);
		obj->SetPos(
			static_cast<float>(GetMouse()->GetInfo().x) - static_cast<int>(ScrollManager::GetScrollX()),
			static_cast<float>(GetMouse()->GetInfo().y) - static_cast<int>(ScrollManager::GetScrollY()));
		obj->SetCX(_selectImage[_selectCount]->GetWidth());
		obj->SetCY(_selectImage[_selectCount]->GetHeight());

		_list[obj->GetLayer()].emplace_back(obj);
		_selectCount++;
		if (_selectCount >= _selectImage.size())
		{
			_selectCount = 0;
		}
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
			int x = 90 + (90 * countX) + 1024;
			int y = 70 + (70 * countY);
			RECT tile_rc{ x, y, x + static_cast<LONG>(tile.second[0]->GetWidth()), y + static_cast<LONG>(tile.second[0]->GetHeight()) };
			RECT mouse_rc{ pt.x, pt.y, pt.x + 1, pt.y + 1 };
			if (IntersectRect(&rc, &mouse_rc, &tile_rc))
			{
				_selectImage = tile.second;
				_selectFileName = tile.first;
				_selectPath = tiles.first;
				_selectCount = 0;
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
	if (_selectImage.empty()) {
		return;
	}
	GetMouse()->SetCX(_selectImage[_selectCount]->GetWidth());
	GetMouse()->SetCY(_selectImage[_selectCount]->GetHeight());
	GetMouse()->UpdateRect();
	int countX = 0;
	int countY = 0;
	for (auto tiles : _images)
	{
		for (auto tile : tiles.second)
		{
			RECT rc{};
			int x = 90 + (90 * countX) + 1024;
			int y = 70 + (70 * countY);
			GetMouse()->SetInfo({
				GetMouse()->GetInfo().x - ScrollManager::GetScrollX(),
				GetMouse()->GetInfo().y - ScrollManager::GetScrollY(),
				GetMouse()->GetInfo().cx,
				GetMouse()->GetInfo().cy });
			for (auto objs : _list)
			{
				for (auto obj : objs)
				{
					obj->UpdateRect();
					CollisionManager::Collision_RectEX<MapObject>(obj, GetMouse());
				}
			}
			GetMouse()->SetInfo({
				GetMouse()->GetInfo().x + ScrollManager::GetScrollX(),
				GetMouse()->GetInfo().y + ScrollManager::GetScrollY(),
				GetMouse()->GetInfo().cx,
				GetMouse()->GetInfo().cy });

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


	WriteFile(hFile, &_mapSize, sizeof(_mapSize), &dwByte, nullptr);
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
			WriteFile(hFile, &info, sizeof(info), &dwByte, nullptr);
			auto rect = obj->GetRect();
			WriteFile(hFile, &rect, sizeof(rect), &dwByte, nullptr);
			uint32_t layer = obj->GetLayer();
			WriteFile(hFile, &layer, sizeof(layer), &dwByte, nullptr);
			uint32_t number = obj->GetImageNumber();
			WriteFile(hFile, &number, sizeof(number), &dwByte, nullptr);
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


	check = ReadFile(hFile, &_mapSize, sizeof(_mapSize), &dwByte, nullptr);
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
			uint32_t number;
			check = ReadFile(hFile, &number, sizeof(number), &dwByte, nullptr);
			obj->SetImageNumber(number);
			
			std::string fullPath;
			fullPath.append(obj->GetPath()).append("\\").append(obj->GetFileName()).append(".").
				append(std::to_string(obj->GetImageNumber())).append(".bmp");
			auto image = _images.find(obj->GetPath());
			auto file = image->second.find(obj->GetFileName());
			auto tile = file->second[obj->GetImageNumber()];
			obj->SetImage(tile);
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

void MapManager::TileLoad(std::wstring name)
{
	std::string tiles[]{ "bsc", "edD", "edU", "enH0", "enH1", "enV0", "enV1", "slLD", "slLU", "slRD", "slRU" };

	std::map<std::string, std::vector<MyBitmap*>> list;
	for (auto& tileName : tiles)
	{
		for (int i = 0; i < 15; i++)
		{
			char path[100];
			snprintf(path, 100, "Client\\Map\\Tile\\%s\\%s.%d.bmp", StringTools::WStringToString(name.c_str()).c_str(), tileName.c_str(), i);

			if (!_access(path, 0))
			{
				MyBitmap* image = new MyBitmap;
				image->Insert_Bitmap(_hWnd, StringTools::StringToWString(path).c_str());
				list[tileName].push_back(image);
			}
			else
			{
				break;
			}
		}
	}
	_images.insert({ StringTools::WStringToString(name.c_str()), list });
}

Mouse* MapManager::GetMouse()
{
	return _mouse;
}
