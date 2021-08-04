#include "../../pch.h"
#include <io.h>
#include "../../Components/Base/game_object.h"
#include "../../Components/MapObject/map_object_info.h"
#include "../../Components/MapObject/foot_hold.h"
#include "../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../../Common/Managers/CollisionManager/Collision_Manager.h"
#include "../../../Common/Utility/file_manager.h"
#include "../ScrollManager/scroll_manager.h"
#include "map_manager.h"

void MapManager::ReadyMapManager()
{

	std::string tiles[]{ "bsc", "edD", "edU", "enH0", "enH1", "enV0", "enV1", "slLD", "slLU", "slRD", "slRU" };

	std::map<std::string, std::vector<MyBitmap*>> list;
	for (auto& tileName : tiles)
	{
		for (int i = 0; i < 15; i++)
		{
			char path[100];
			snprintf(path, 100, "Client\\Map\\Tile\\woodMarble.img\\%s.%d.bmp", tileName.c_str(), i);

			if (!_access(path, 0))
			{
				MyBitmap* image = new MyBitmap;
				image->Insert_Bitmap(_hWnd, StringTools::StringToWString(path).c_str());
				list[tileName].push_back(image);
				std::cout << path << std::endl;
			}
			else
			{
				break;
			}
		}
	}
	_listBitmap.insert({ "woodMarble.img", list });

	//auto fileNams = FileManager::GetInstance()->GetDirFileName(L"Client\\Map\\Tile\\woodMarble.img\\");

	//std::map<std::string, MyBitmap*> list;
	//for (auto begin : fileNams) {
	//	MyBitmap* image = new MyBitmap;
	//	image->Insert_Bitmap(_hWnd, begin.c_str());
	//	list.insert({ FileManager::GetInstance()->GetFileName(StringTools::WStringToString(begin.c_str())), image });
	//}
	//_listBitmap.insert(std::make_pair("woodMarble.img", list));
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
			uint32_t number;
			check = ReadFile(hFile, &number, sizeof(number), &dwByte, nullptr);
			obj->SetImageNumber(number);


			std::string fullPath;
			fullPath.append(obj->GetPath()).append("\\").append(obj->GetFileName()).append(".").
				append(std::to_string(obj->GetImageNumber())).append(".bmp");
			auto image = _listBitmap.find(obj->GetPath());
			auto file = image->second.find(obj->GetFileName());
			auto tile = file->second[obj->GetImageNumber()];
			obj->SetImage(tile);
			_listGameObject[layer].push_back(obj);

			//std::string fullPath;
			//fullPath.append(obj->GetPath()).append(obj->GetPath());
			//auto image = _listBitmap.find(obj->GetPath());
			//if (image != _listBitmap.end())
			//{
			//	auto file = image->second.find(obj->GetFileName());
			//	if (file != image->second.end())
			//	{
			//		obj->SetImage(file->second);
			//	}
			//}
			//_listGameObject[layer].push_back(obj);
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

void MapManager::RenderFootHoldManager(HDC hDC)
{
	for (auto iter = _listFootHold.begin(); iter != _listFootHold.end(); ++iter)
	{
		(*iter)->RenderFootHold(hDC);
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

bool MapManager::FootholdYCollision(GameObject* object, float* outY)
{
	if (_listFootHold.empty())
		return false;

	FootHold* pTarget = nullptr;

	for (FootHold* footHold : _listFootHold)
	{
		if (object->GetInfo().x >= footHold->GetStartPos().x &&
			object->GetInfo().x <= footHold->GetEndPos().x)
			pTarget = footHold;
	}
	if (nullptr == pTarget)
		return false;

	// PlayerY = ((y2 - y1) / (x2 - x1)) * (PlayerX - x1) + y1

	float x1 = static_cast<float>(pTarget->GetStartPos().x);
	float y1 = static_cast<float>(pTarget->GetStartPos().y);
	float x2 = static_cast<float>(pTarget->GetEndPos().x);
	float y2 = static_cast<float>(pTarget->GetEndPos().y);

	*outY = ((y2 - y1) / (x2 - x1)) * (object->GetInfo().x - x1) + y1;
	*outY -= (object->GetInfo().cy >> 1);
	return true;
	//*outY -= (object->GetInfo().cy >> 1);
	//float myY = object->GetInfo().y + (object->GetInfo().cy >> 1);
	//if (*outY > myY)
	//{
	//	//*outY -= (object->GetInfo().cy >> 1);
	//	return true;
	//}
	//else
	//{
	//	return false;
	//}
	//if (_listFootHold.empty())
	//	return false;


	//for (FootHold* footHold : _listFootHold)
	//{
	//	if (object->GetInfo().x >= footHold->GetStartPos().x && object->GetInfo().x <= footHold->GetEndPos().x)
	//	{
	//		double x1 = footHold->GetStartPos().x;
	//		double y1 = footHold->GetStartPos().y;
	//		double x2 = footHold->GetEndPos().x;
	//		double y2 = footHold->GetEndPos().y;

	//		double result1 = ((y2 - y1) / (x2 - x1)) * (object->GetInfo().x - (object->GetInfo().cx / 2) - x1) + y1 - (object->GetInfo().cy / 2);
	//		double result2 = ((y2 - y1) / (x2 - x1)) * (object->GetInfo().x + (object->GetInfo().cx / 2) - x1) + y1 - (object->GetInfo().cy / 2);

	//		if ((result1 < object->GetInfo().y && result1 + (object->GetInfo().cy / 2) + 1.f > object->GetInfo().y) ||
	//			(result2 < object->GetInfo().y && result2 + (object->GetInfo().cy / 2) + 1.f > object->GetInfo().y))
	//		{
	//			*outY = static_cast<float>((result1 + result2) / 2);
	//			//ScrollManager::SetScrollY(info.y - moveY);
	//			//info.y = moveY;
	//			return true;
	//		}
	//	}
	//}
}

bool MapManager::FootholdAndRectCollision(GameObject* object)
{

	if (_listFootHold.empty())
		return false;

	FootHold* pTarget = nullptr;

	for (FootHold* footHold : _listFootHold)
	{
		float x = static_cast<float>(footHold->GetEndPos().x) - static_cast<float>(footHold->GetStartPos().x);
		float y = static_cast<float>(footHold->GetEndPos().y) - static_cast<float>(footHold->GetStartPos().y);
		float radian = std::atan2(y, x);
		float degree = radian * 180 / 3.141592f;
		if (degree < 0) {
			degree *= -1;
		}

		if (degree >= 85 && degree <= 95 ||
			degree >= 175 && degree <= 195)
		{
			if (footHold->GetEndPos().x <= object->GetRect().right &&
				footHold->GetStartPos().x >= object->GetRect().left &&
				footHold->GetEndPos().y <= object->GetRect().bottom &&
				footHold->GetStartPos().y >= object->GetRect().top)
			{ // 선과 사각형의 충돌.

				return true;
			}
			else if (footHold->GetStartPos().x <= object->GetRect().right &&
				footHold->GetEndPos().x >= object->GetRect().left &&
				footHold->GetStartPos().y <= object->GetRect().bottom &&
				footHold->GetEndPos().y >= object->GetRect().top)
			{
				return true;
			}
		}

	}



	return false;
}
