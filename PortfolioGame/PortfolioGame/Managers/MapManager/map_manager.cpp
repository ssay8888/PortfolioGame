#include "../../pch.h"
#include "../../Components/Base/game_object.h"
#include "../../Components/MapObject/map_object_info.h"
#include "../../Components/MapObject/foot_hold.h"
#include "../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../../Common/Utility/file_manager.h"
#include "../ScrollManager/scroll_manager.h"
#include "map_manager.h"

void MapManager::ReadyMapManager()
{
	auto fileNams = FileManager::GetInstance()->GetDirFileName(L"Client\\Map\\Tile\\woodMarble.img\\");

	std::map<std::string, MyBitmap*> list;
	for (auto begin : fileNams) {
		MyBitmap* image = new MyBitmap;
		image->Insert_Bitmap(_hWnd, begin.c_str());
		list.insert({ FileManager::GetInstance()->GetFileName(StringTools::WStringToString(begin.c_str())), image });
	}
	_listBitmap.insert(std::make_pair("Client\\Map\\Tile\\woodMarble.img\\", list));
}

void MapManager::AddGameObject(GameObject* object)
{
	_listGameObject[object->GetLayer()].emplace_back(object);
}

void MapManager::AddFootHold(FootHold* foothold)
{
	_listFootHold.push_back(foothold);
}

void MapManager::LoadMapData()
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
		for (int i = 0; i < size; i++)
		{
			MapObjectInfo* obj = new MapObjectInfo(0);
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
			fullPath.append(obj->GetPath()).append(obj->GetPath());
			auto image = _listBitmap.find(obj->GetPath());
			if (image != _listBitmap.end())
			{
				auto file = image->second.find(obj->GetFileName());
				if (file != image->second.end())
				{
					obj->SetImage(file->second);
				}
			}
			_listGameObject[layer].push_back(obj);
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
		_listFootHold.push_back(footHold);
	}

	CloseHandle(hFile);
	//MessageBox(nullptr, L"로드 성공", L"확인", MB_OK);
}

void MapManager::ReleaseAllData()
{
	for (int i = 0; i < MaxLayer; i++)
	{
		for (auto object : _listGameObject[i])
		{
			delete object;
		}
		_listGameObject[i].clear();
	}

	for (auto foot : _listFootHold)
	{
		delete foot;
	}
}

void MapManager::UpdateGameObjectManager(const float deltaTime)
{
	for (int i = 0; i < MaxLayer; i++)
	{
		for (auto iter = _listGameObject[i].begin(); iter != _listGameObject[i].end();)
		{
			GameObject::State iEvent = (*iter)->DoUpdateObject(deltaTime);
			if (GameObject::State::kDead == iEvent)
			{
				if ((*iter))
				{
					delete (*iter);
					(*iter) = nullptr;
				}
				iter = _listGameObject[i].erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}
}

void MapManager::RenderGameObjectManager(HDC hdc)
{

	for (int i = 0; i < MaxLayer; i++)
	{
		for (auto& pObject : _listGameObject[i])
		{
			pObject->DoRenderGameObject(hdc);
		}
	}
}

void MapManager::LateUpdateGameObjectManager()
{
	for (int i = 0; i < MaxLayer; i++)
	{
		for (auto iter = _listGameObject[i].begin(); iter != _listGameObject[i].end(); ++iter)
		{
			GameObject::State iEvent = (*iter)->DoLateUpdateGameObject();
		}
	}
}

void MapManager::ReleaseGameObjectManager()
{
}

bool MapManager::FootholdCollision(Info& info, float* outY)
{

	if (_listFootHold.empty())
		return false;


	for (FootHold* footHold : _listFootHold)
	{
		if (info.x >= footHold->GetStartPos().x && info.x <= footHold->GetEndPos().x)
		{
			double x1 = footHold->GetStartPos().x;
			double y1 = footHold->GetStartPos().y;
			double x2 = footHold->GetEndPos().x;
			double y2 = footHold->GetEndPos().y;

			double result1 = ((y2 - y1) / (x2 - x1)) * (info.x - (info.cx / 2) - x1) + y1 - (info.cy / 2);
			double result2 = ((y2 - y1) / (x2 - x1)) * (info.x + (info.cx / 2) - x1) + y1 - (info.cy / 2);

			if ((result1 < info.y && result1 + (info.cy / 2) + 1.f > info.y) ||
				(result2 < info.y && result2 + (info.cy / 2) + 1.f > info.y))
			{
				*outY = static_cast<float>((result1 + result2) / 2);
				//ScrollManager::SetScrollY(info.y - moveY);
				//info.y = moveY;
				return true;
			}
		}
	}
	return false;
}
