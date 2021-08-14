#include "../../pch.h"
#include <io.h>
#include "Map/map_instance.h"
#include "../../Components/Base/game_object.h"
#include "../../Components/MapObject/map_object_info.h"
#include "../../Components/MapObject/portal.h"
#include "../../Components/MapObject/Player/player.h"
#include "../../Components/MapObject/Monster/monster.h"
#include "../../Components/MapObject/foot_hold.h"
#include "../../Components/MapObject/ani_map_object.h"
#include "../../Managers/MonsterMnager/monster_manager.h"
#include "../../../Common/Managers/BitmapManager/my_bitmap.h"
#include "../../../Common/Managers/CollisionManager/Collision_Manager.h"
#include "../../../Common/Utility/file_manager.h"
#include "../ScrollManager/scroll_manager.h"
#include "../../Utility/xml_reader.h"
#include "map_manager.h"

MapManager::MapManager() :
	_map_player(nullptr),
	_now_map(nullptr)
{
}

void MapManager::ReadyMapManager()
{
	std::string tileFolder[] = {"woodMarble.img", "darkGrassySoil.img"};
	for (auto& folder : tileFolder)
	{
		TileImageLoad(folder);
	}
	MapObjectImageLoad();
}

void MapManager::LoadMapData(uint32_t mapid)
{
	wchar_t path[255];
	swprintf_s(path, 255, L"../Data/%d.dat", mapid);
	
	HANDLE hFile = CreateFile(path,
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
	
	std::shared_ptr<MapInstance*> map_instance = std::make_shared<MapInstance*>(new MapInstance());
	MapInstance* map = (*map_instance);

	DWORD dwByte = 0;

	unsigned short mark = 0xFEFF;
	bool  check = ReadFile(hFile, &mark, sizeof(mark), &dwByte, nullptr);
	ObjectPos mapSize;
	check = ReadFile(hFile, &mapSize, sizeof(mapSize), &dwByte, nullptr);
	map->SetMapSize(mapSize);
	for (int i = 0; i < 7; i++)
	{
		size_t size;
		check = ReadFile(hFile, &size, sizeof(size), &dwByte, nullptr);
		for (int j = 0; j < size; j++)
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
			GameObject::ObjectType type;
			check = ReadFile(hFile, &type, sizeof(type), &dwByte, nullptr);
			obj->SetObjectType(type);

			if (obj->GetImageNumber() == -1)
			{
				auto image = _listObjBitmap.find(obj->GetPath());
				obj->SetImage(image->second);
				map->AddGameObject(obj);
			} 
			else if (obj->GetImageNumber() == -2)
			{

				auto monster = MonsterManager::GetInstance()->FindMonster(obj->GetPath());
				auto monsterCopy = new Monster(*(*monster));
				monsterCopy->SetInfo(obj->GetInfo());
				monsterCopy->DoReadyGame();

				map->AddGameObject(monsterCopy);
				map->AddMonsterObject(monsterCopy);
				delete obj;
			}
			else
			{
				std::string fullPath;
				fullPath.append(obj->GetPath()).append("\\").append(obj->GetFileName()).append(".").
					append(std::to_string(obj->GetImageNumber())).append(".bmp");
				auto image = _listBitmap.find(obj->GetPath());
				auto file = image->second.find(obj->GetFileName());
				auto tile = file->second[obj->GetImageNumber()];
				obj->SetImage(tile);

				map->AddGameObject(obj);
			}

		}
	}

	size_t footSize;
	check = ReadFile(hFile, &footSize, sizeof(footSize), &dwByte, nullptr);
	for (int i = 0; i < footSize; i++)
	{
		FootHold* footHold = new FootHold();
		ObjectPos startPos;
		ObjectPos endPos;
		check = ReadFile(hFile, &startPos, sizeof(startPos), &dwByte, nullptr);
		check = ReadFile(hFile, &endPos, sizeof(endPos), &dwByte, nullptr);
		footHold->SetStartPos(startPos.x, startPos.y);
		footHold->SetEndPos(endPos.x, endPos.y);
		map->AddFootHold(footHold);
	}

	size_t ropeSize;
	check = ReadFile(hFile, &ropeSize, sizeof(ropeSize), &dwByte, nullptr);
	for (int i = 0; i < ropeSize; i++)
	{
		FootHold* footHold = new FootHold();
		ObjectPos startPos;
		ObjectPos endPos;
		auto state = footHold->GetState();
		check = ReadFile(hFile, &state, sizeof(state), &dwByte, nullptr);
		check = ReadFile(hFile, &startPos, sizeof(startPos), &dwByte, nullptr);
		check = ReadFile(hFile, &endPos, sizeof(endPos), &dwByte, nullptr);
		footHold->SetStartPos(startPos.x, startPos.y);
		footHold->SetEndPos(endPos.x, endPos.y);
		footHold->SetState(state);
		map->AddRopeLadder(footHold);
	}

	CloseHandle(hFile);
	if (mapid == 0)
	{ 
		Info info1{ 285, 500, 87, 182 };
		auto portal1 = new Portal({ 285, 400 }, 1, info1);
		map->AddPortal(portal1);
	}
	else if (mapid == 1)
	{
		Info info2{ 285, 400, 87, 182 };
		auto portal2 = new Portal({ 285, 500 }, 0, info2);
		map->AddPortal(portal2);
	}

	_listMap.insert(std::make_pair(mapid, map_instance));
	//MessageBox(nullptr, L"로드 성공", L"확인", MB_OK);
}

void MapManager::ReleaseAllData()
{
	for (auto& data : _listBitmap)
	{
		for (auto& data2 : data.second)
		{
			auto bitmaps = data2.second;
			for (auto bitmap : bitmaps)
			{
				delete bitmap;
			}
		}
	}
	for (auto& data : _listObjBitmap)
	{
		delete data.second;
	}
	_listObjBitmap.clear();
}

void MapManager::UpdateGameObjectManager(const float deltaTime)
{
	
	auto now_map = (*GetNowMap());
	for (int i = 0; i < MaxLayer; i++)
	{
		auto& list = now_map->GetGameObjectList(i);
		for (auto iter = list.begin(); iter != list.end();)
		{
			GameObject::State iEvent = (*iter)->DoUpdateObject(deltaTime);
			if (GameObject::State::kDead == iEvent)
			{
				if ((*iter))
				{
					delete (*iter);
					(*iter) = nullptr;
				}
				iter = list.erase(iter);
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
	auto now_map = (*GetNowMap());
	Rectangle(hdc,
		0 + static_cast<int>(ScrollManager::GetScrollX()),
		0 + static_cast<int>(ScrollManager::GetScrollY()),
		static_cast<int>(now_map->GetMapSize().x + ScrollManager::GetScrollX()),
		static_cast<int>(now_map->GetMapSize().y + ScrollManager::GetScrollY()));
		now_map->GetBackGroundImage()->RenderBitmapImage(hdc,
		static_cast<int>(0),
		static_cast<int>(0),
		800, 600);

	for (int i = 0; i < MaxLayer; i++)
	{
		auto data = now_map->GetGameObjectList(i);
		for (auto& pObject : data)
		{
			float startPosX = ScrollManager::GetScrollX();
			float startPosY = ScrollManager::GetScrollY();

			if (pObject->GetInfo().x < -startPosX + -100 ||
				pObject->GetInfo().x > -startPosX + 1300)
			{
				continue;
			}

			if (pObject->GetInfo().y < -startPosY + -200 ||
				pObject->GetInfo().y > -startPosY + 900)
			{
				continue;
			}


			pObject->DoRenderGameObject(hdc);
		}
	}

	for (auto& data : now_map->GetPortalList())
	{
		data->RenderPortal(hdc);
	}


}

void MapManager::RenderFootHoldManager(HDC hDC)
{
	for (auto iter = (*GetNowMap())->GetFootHoldList().begin(); iter != (*GetNowMap())->GetFootHoldList().end(); ++iter)
	{
		(*iter)->RenderFootHold(hDC);
	}
	for (auto iter = (*GetNowMap())->GetRopeLadderList().begin(); iter != (*GetNowMap())->GetRopeLadderList().end(); ++iter)
	{
		(*iter)->RenderFootHold(hDC);
	}
}

void MapManager::LateUpdateGameObjectManager()
{
	for (int i = 0; i < MaxLayer; i++)
	{
		for (auto iter : (*GetNowMap())->GetGameObjectList(i))
		{
			GameObject::State iEvent = iter->DoLateUpdateGameObject();
		}
	}
}

void MapManager::ReleaseGameObjectManager()
{
}

bool MapManager::FootholdYCollision(GameObject* object, float* outY, FootHold** outHold)
{
	if ((*GetNowMap())->GetFootHoldList().empty())
		return false;

	FootHold* pTarget = nullptr;


	for (FootHold* footHold : (*GetNowMap())->GetFootHoldList())
	{
		float xas = footHold->GetStartPos().x;
		if (object->GetInfo().x >= footHold->GetStartPos().x &&
			object->GetInfo().x <= footHold->GetEndPos().x)
		{

			float x = static_cast<float>(footHold->GetEndPos().x) - static_cast<float>(footHold->GetStartPos().x);
			float y = static_cast<float>(footHold->GetEndPos().y) - static_cast<float>(footHold->GetStartPos().y);
			float radian = std::atan2(y, x);
			float degree = radian * 180 / 3.141592f;
			if (!(degree >= 85 && degree <= 95) &&
				!(degree >= 175 && degree <= 195))
			{
				float characterY = object->GetInfo().y + (object->GetInfo().cy / 2) - 1;
				if (characterY < footHold->GetEndPos().y ||
					characterY < footHold->GetStartPos().y)
				{
					if (pTarget != nullptr)
					{
						float fDistY = std::fabs(pTarget->GetStartPos().y - object->GetInfo().y);
						float fDistY2 = std::fabs(footHold->GetStartPos().y - object->GetInfo().y);
 
						//std::cout << fDistY << " : " << fDistY2 << std::endl;
						if (fDistY < 0 || fDistY < fDistY2)
						{
							continue;
						}

					}
					pTarget = footHold;
				}
			}

		}
	}
	if (pTarget == nullptr)
	{
		for (FootHold* footHold : (*GetNowMap())->GetFootHoldList())
		{
			float xas = footHold->GetStartPos().x;
			// 528
			// 249-1093
			if (object->GetInfo().x <= footHold->GetStartPos().x &&
				object->GetInfo().x >= footHold->GetEndPos().x)
			{

				float x = static_cast<float>(footHold->GetStartPos().x) - static_cast<float>(footHold->GetEndPos().x);
				float y = static_cast<float>(footHold->GetStartPos().y) - static_cast<float>(footHold->GetEndPos().y);
				float radian = std::atan2(y, x);
				float degree = radian * 180 / 3.141592f;
				if (!(degree >= 85 && degree <= 95) &&
					!(degree >= 175 && degree <= 195))
				{
					float characterY = object->GetInfo().y + (object->GetInfo().cy / 2) - 1;
					if (characterY < footHold->GetEndPos().y ||
						characterY < footHold->GetStartPos().y)
					{
						if (pTarget != nullptr)
						{
							float fDistY = std::fabs(pTarget->GetStartPos().y - object->GetInfo().y);
							float fDistY2 = std::fabs(footHold->GetStartPos().y - object->GetInfo().y);

							if (fDistY < 0 || fDistY < fDistY2)
							{
								continue;
							}

						}
						pTarget = footHold;
					}
				}

			}
		}
	}
	if (nullptr == pTarget)
	{
		*outHold = nullptr;
		return false;
	}
	float x1 = static_cast<float>(pTarget->GetStartPos().x);
	float y1 = static_cast<float>(pTarget->GetStartPos().y);
	float x2 = static_cast<float>(pTarget->GetEndPos().x);
	float y2 = static_cast<float>(pTarget->GetEndPos().y);
	*outY = ((y2 - y1) / (x2 - x1)) * (object->GetInfo().x - x1) + y1;
	*outY -= (object->GetInfo().cy >> 1);
	*outHold = pTarget;
	return true;
}

bool MapManager::FootholdAndRectCollision(GameObject* object)
{
	//90도정도의 라인이라면 길을막는다.
	if ((*GetNowMap())->GetFootHoldList().empty())
		return false;

	FootHold* pTarget = nullptr;

	for (FootHold* footHold : (*GetNowMap())->GetFootHoldList())
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
			//std::cout << "가지면안되는데..?" << degree << std::endl;
			if (CollisionManager::LineAndRectCollsition(footHold, object))
			{
				return true;
			}
		}

	}
	return false;
}

bool MapManager::MonsterHitBoxCollision(const RECT hit_box)
{
	RECT rc;
	RECT player = _map_player->GetRect();
	if (IntersectRect(&rc, &player, &hit_box))
	{
		return true;
	}
	return false;
}

bool MapManager::LadderRopeCollsition(GameObject* object, float* outX, FootHold** outHold)
{
	if ((*GetNowMap())->GetRopeLadderList().empty())
		return false;

	FootHold* pTarget = nullptr;


	for (FootHold* footHold : (*GetNowMap())->GetRopeLadderList())
	{
		float x = static_cast<float>(footHold->GetEndPos().x) - static_cast<float>(footHold->GetStartPos().x);
		float y = static_cast<float>(footHold->GetEndPos().y) - static_cast<float>(footHold->GetStartPos().y);

		if (footHold->GetEndPos().x <= object->GetInfo().x +10 &&
			footHold->GetStartPos().x >= object->GetInfo().x - 10 &&
			footHold->GetEndPos().y <= object->GetRect().bottom &&
			footHold->GetStartPos().y >= object->GetRect().top)
		{ // 선과 사각형의 충돌.

			*outX = footHold->GetStartPos().x;
			*outHold = footHold;
			return true;
		}
		else if (footHold->GetStartPos().x <= object->GetInfo().x + 10 &&
			footHold->GetEndPos().x >= object->GetInfo().x  -10 &&
			footHold->GetStartPos().y <= object->GetRect().bottom &&
			footHold->GetEndPos().y >= object->GetRect().top)
		{
			*outX = footHold->GetStartPos().x;
			*outHold = footHold;
			return true;
		}

	}
	return false;
}

bool MapManager::PortalCollsition(GameObject* object, Portal** outPortal)
{
	RECT rc;
	auto map = (*GetNowMap());
	for (auto portal : map->GetPortalList())
	{
		RECT portalRect = portal->GetRect();
		RECT objectRect = object->GetRect();
		if (IntersectRect(&rc, &portalRect, &objectRect))
		{
			*outPortal = portal;
			std::cout << "포탈충돌" << std::endl;
			return true;
		}
	}
	return false;
}

void MapManager::TileImageLoad(std::string folderName)
{
	std::string tiles[]{ "bsc", "edD", "edU", "enH0", "enH1", "enV0", "enV1", "slLD", "slLU", "slRD", "slRU" };

	std::map<std::string, std::vector<MyBitmap*>> list;
	for (auto& tileName : tiles)
	{
		for (int i = 0; i < 15; i++)
		{
			char path[100];
			snprintf(path, 100, "Client\\Map\\Tile\\%s\\%s.%d.bmp", folderName.c_str(), tileName.c_str(), i);

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
	_listBitmap.insert({ folderName, list });
}

void MapManager::InsertObjectImage(std::string path, AniMapObject* object)
{
	_listAniObject.insert({ path, object });
}

AniMapObject* MapManager::FindAniObjectCopy(std::string key)
{
	auto data = _listAniObject.find(key);
	if (data != _listAniObject.end())
	{
		auto obj = new AniMapObject((*data->second));
		return obj;
	}
	return nullptr;
}

std::shared_ptr<MapInstance*> MapManager::GetNowMap()
{
	return _now_map;
}

void MapManager::AniObjectImageLoad()
{
	XmlReader::GetInstance().LoadPortal();
}

void MapManager::SetPlayer(Player* player)
{
	_map_player = player;
}

Player* MapManager::GetPlayer()
{
	return _map_player;
}

void MapManager::ChangeMap(int32_t next_map, ObjectPos pos)
{
	auto map = _listMap.find(next_map);
	if (map != _listMap.end())
	{
		(*map->second)->SetPlayer(_map_player);
		_now_map = map->second;
		_map_player->SetInfo({pos.x, pos.y, _map_player->GetInfo().cx, _map_player->GetInfo().cy});
	}
}

void MapManager::MapObjectImageLoad()
{
	auto files = FileManager::GetInstance()->GetDirFileName(L"Client\\Map\\Obj\\");

	for (auto wpath : files)
	{
		auto path = StringTools::WStringToString(wpath.c_str());
		if (!_access(path.c_str(), 0))
		{
			MyBitmap* image = new MyBitmap;
			image->Insert_Bitmap(_hWnd, wpath.c_str());
			_listObjBitmap.insert(std::make_pair(StringTools::WStringToString(wpath.c_str()), image));
		}
	}
}

std::list<Monster*> MapManager::MonsterCollision(RECT rect, uint32_t count)
{
	RECT rc;
	std::list<Monster*> monsters;
	for (int i = 0; i < MaxLayer; ++i)
	{
		auto list = (*GetNowMap())->InMapMonsterObjectList(i);
		for (auto object : *list)
		{
			if (monsters.size() >= count) {
				break;
			}
			auto data = dynamic_cast<Monster*>(object);
			if (data != nullptr)
			{
				if (!data->IsAlive())
					continue;
				RECT dist = data->GetRect();
				if (IntersectRect(&rc, &rect, &dist))
				{
					monsters.emplace_back(data);
				}
			}
		}
		delete list;
	}
	return monsters;
}
