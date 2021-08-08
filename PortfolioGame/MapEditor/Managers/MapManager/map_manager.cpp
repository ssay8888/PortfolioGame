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
	_mapSize({1024, 768}),
	_selectState(SelectState::kTile)
{
}

void MapManager::Ready_Map()
{
	std::wstring tiles[]{ L"woodMarble.img", L"darkGrassySoil.img" };

	for (auto tile : tiles)
	{
		TileLoad(tile);
	}
	MapObjectImageLoad();
	EtcImageLoad();
	ButtonLoad();
	MonsterImageLoad();
	_mouse = new Mouse();
}

void MapManager::Update_Map()
{
	POINT pt{};
	GetCursorPos(&pt);
	ScreenToClient(_hWnd, &pt);
	GetMouse()->SetPos(static_cast<float>(pt.x), static_cast<float>(pt.y));
	if (!_selectTileImage.empty())
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
			SelectButtonImage(pt);
			switch (_selectState)
			{
			case MapManager::SelectState::kTile:
				SelectTileImage(pt);
				break;
			case MapManager::SelectState::kObject:
				SelectObjectImage(pt);
				break;
			case MapManager::SelectState::kEtc:
				SelectEtcImage(pt);
				break;
			case MapManager::SelectState::kMob:
				SelectMobImage(pt);
				break;
			default:
				break;
			}
		}
	}
	else if (KeyManager::GetInstance()->KeyDown(KEY_RBUTTON))
	{
		switch (_selectState)
		{
		case MapManager::SelectState::kTile:
		{
			auto foot = new FootHold();
			foot->SetStartPos(static_cast<float>(pt.x - ScrollManager::GetScrollX()), static_cast<float>(pt.y - ScrollManager::GetScrollY()));
			_footholds.emplace_back(foot);
			break;
		}
		case MapManager::SelectState::kEtc:
		{
			if (_selectEtcImage.second != nullptr)
			{
				auto foot = new FootHold();
				foot->SetStartPos(static_cast<float>(pt.x - ScrollManager::GetScrollX()), static_cast<float>(pt.y - ScrollManager::GetScrollY()));
				if (_selectEtcImage.first.find("ladder") != std::string::npos)
				{
					foot->SetState(FootHold::HoldState::kLadder);
					_ladderRopes.push_back(foot);
				}
				else if (_selectEtcImage.first.find("rope") != std::string::npos)
				{
					foot->SetState(FootHold::HoldState::kRope);
					_ladderRopes.push_back(foot);
				}
				else
				{
					delete foot;
				}
			}
			break;
		}
		default:
			break;
		}
	}
	else if (KeyManager::GetInstance()->KeyPressing(KEY_RBUTTON))
	{
		switch (_selectState)
		{
		case MapManager::SelectState::kTile:
			_footholds.back()->SetEndPos(static_cast<float>(pt.x - ScrollManager::GetScrollX()), static_cast<float>(pt.y - ScrollManager::GetScrollY()));
			break;
		case MapManager::SelectState::kEtc:
			if (_selectEtcImage.second != nullptr)
			{
				if (_selectEtcImage.first.find("ladder") != std::string::npos ||
					_selectEtcImage.first.find("rope") != std::string::npos)
				{
					_ladderRopes.back()->SetEndPos(static_cast<float>(pt.x - ScrollManager::GetScrollX()), static_cast<float>(pt.y - ScrollManager::GetScrollY()));
				}
				else if (_selectEtcImage.first.find("SizeXDown") != std::string::npos)
				{
					_mapSize.x -= 10;
				}
				else if (_selectEtcImage.first.find("SizeXUp") != std::string::npos)
				{
					_mapSize.x += 10;
				}
				else if (_selectEtcImage.first.find("SizeYDown") != std::string::npos)
				{
					_mapSize.y -= 10;
				}
				else if (_selectEtcImage.first.find("SizeYUp") != std::string::npos)
				{
					_mapSize.y += 10;
				}
			}
			break;
		default:
			break;
		}
	}
	else if (KeyManager::GetInstance()->KeyUp(KEY_RBUTTON))
	{
		switch (_selectState)
		{
		case MapManager::SelectState::kTile:
			_footholds.back()->SetEndPos(static_cast<float>(pt.x - ScrollManager::GetScrollX()), static_cast<float>(pt.y - ScrollManager::GetScrollY()));
			break;
		case MapManager::SelectState::kEtc:
			if (_selectEtcImage.second != nullptr)
			{
				if (_selectEtcImage.first.find("ladder") != std::string::npos ||
					_selectEtcImage.first.find("rope") != std::string::npos)
				{
					_ladderRopes.back()->SetEndPos(static_cast<float>(pt.x - ScrollManager::GetScrollX()), static_cast<float>(pt.y - ScrollManager::GetScrollY()));
				}
			}
			break;
		default:
			break;
		}
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

void MapManager::Render_Map(HDC hdc)
{
	POINT pt{};
	GetCursorPos(&pt);
	ScreenToClient(_hWnd, &pt);

	int countX = 0;
	int countY = 0;

	Rectangle(hdc, 
		0 + static_cast<int>(ScrollManager::GetScrollX()), 
		0 + static_cast<int>(ScrollManager::GetScrollY()), 
		static_cast<int>(_mapSize.x + ScrollManager::GetScrollX()),
		static_cast<int>(_mapSize.y + ScrollManager::GetScrollY()));
	for (auto& maps : _list)
	{
		for (auto& map : maps)
		{
			map->GetImage()->RenderBitmapImage(hdc,
				static_cast<int>(map->GetInfo().x + static_cast<int>(ScrollManager::GetScrollX() - (map->GetImage()->GetWidth() / 2))),
				static_cast<int>(map->GetInfo().y + static_cast<int>(ScrollManager::GetScrollY() - (map->GetImage()->GetHeight() / 2))),
				static_cast<int>(map->GetImage()->GetWidth()),
				static_cast<int>(map->GetImage()->GetHeight()));
		}
	}

	FootHoldRender(hdc);
	LadderRopeRender(hdc);


	Rectangle(hdc, 1024, 0, 1600, 900);

	switch (_selectState)
	{
	case MapManager::SelectState::kTile:
		if (!_selectTileImage.empty())
		{
			_selectTileImage[_selectCount]->RenderBitmapImage(hdc,
				static_cast<int>(GetMouse()->GetInfo().x - (_selectTileImage[_selectCount]->GetWidth() / 2)),
				static_cast<int>(GetMouse()->GetInfo().y - (_selectTileImage[_selectCount]->GetHeight() / 2)),
				static_cast<int>(_selectTileImage[_selectCount]->GetWidth()),
				static_cast<int>(_selectTileImage[_selectCount]->GetHeight()));
		}
		break;
	case MapManager::SelectState::kObject:
		if (_selectObjImage.second != nullptr)
		{
			_selectObjImage.second->RenderBitmapImage(hdc,
				static_cast<int>(GetMouse()->GetInfo().x - (_selectObjImage.second->GetWidth() / 2)),
				static_cast<int>(GetMouse()->GetInfo().y - (_selectObjImage.second->GetHeight() / 2)),
				static_cast<int>(_selectObjImage.second->GetWidth()),
				static_cast<int>(_selectObjImage.second->GetHeight()));
		}
		break;
	case MapManager::SelectState::kEtc:
		if (_selectEtcImage.second != nullptr)
		{
			_selectEtcImage.second->RenderBitmapImage(hdc,
				static_cast<int>(GetMouse()->GetInfo().x - (_selectEtcImage.second->GetWidth() / 2)),
				static_cast<int>(GetMouse()->GetInfo().y - (_selectEtcImage.second->GetHeight() / 2)) - 150,
				static_cast<int>(_selectEtcImage.second->GetWidth()),
				static_cast<int>(_selectEtcImage.second->GetHeight()));
		}
	case MapManager::SelectState::kMob:
		if (_selectMonsterImage.second != nullptr)
		{
			_selectMonsterImage.second->RenderBitmapImage(hdc,
				static_cast<int>(GetMouse()->GetInfo().x - (_selectMonsterImage.second->GetWidth() / 2)),
				static_cast<int>(GetMouse()->GetInfo().y - (_selectMonsterImage.second->GetHeight() / 2)),
				static_cast<int>(_selectMonsterImage.second->GetWidth()),
				static_cast<int>(_selectMonsterImage.second->GetHeight()));
		}
		break;
	default:
		break;
	}
	ButtonRender(hdc);

	switch (_selectState)
	{
	case MapManager::SelectState::kTile:
		TileRender(hdc);
		break;
	case MapManager::SelectState::kObject:
		MapObjectRender(hdc);
		break;
	case MapManager::SelectState::kEtc:
		EtcRender(hdc);
		break;
	case MapManager::SelectState::kMob:
		MonsterRender(hdc);
		break;
	default:
		break;
	}
	std::wstring str;
	for (int i = 0; i < 7; i++)
	{
		str.append(std::to_wstring(_list[i].size())).append(L"개");
	}
	TextOut(hdc, 0, 0, str.c_str(), static_cast<int>(str.size()));
	str.clear();
	str.append(L"X : ").append(std::to_wstring(pt.x - ScrollManager::GetScrollX())).
		append(L"Y : ").append(std::to_wstring(pt.y - ScrollManager::GetScrollY()));
	TextOut(hdc, 0, 20, str.c_str(), static_cast<int>(str.size()));
	str.clear();
	str.append(L"현재 맵 사이즈 X : ").append(std::to_wstring(_mapSize.x)).append(L"  Y : ").append(std::to_wstring(_mapSize.y));
	TextOut(hdc, 0, 40, str.c_str(), static_cast<int>(str.size()));

}

void MapManager::AddListObject()
{
	switch (_selectState)
	{
	case MapManager::SelectState::kTile:
		if (!_selectTileImage.empty())
		{
			MapObject* obj = new MapObject();
			obj->SetLayer(SelectLayer);
			obj->SetImage(_selectTileImage[_selectCount]);
			obj->SetImageNumber(_selectCount);
			obj->SetFileName(_selectFileName);
			obj->SetObjectType(MapObject::ObjectType::kNoLife);
			obj->SetPath(_selectPath);
			obj->SetPos(
				static_cast<float>(GetMouse()->GetInfo().x) - static_cast<int>(ScrollManager::GetScrollX()),
				static_cast<float>(GetMouse()->GetInfo().y) - static_cast<int>(ScrollManager::GetScrollY()));
			obj->SetCX(_selectTileImage[_selectCount]->GetWidth());
			obj->SetCY(_selectTileImage[_selectCount]->GetHeight());

			_list[obj->GetLayer()].emplace_back(obj);
			_selectCount++;
			if (_selectCount >= _selectTileImage.size())
			{
				_selectCount = 0;
			}
		}
		break;
	case MapManager::SelectState::kObject:
		if (_selectObjImage.second != nullptr)
		{
			MapObject* obj = new MapObject();

			obj->SetLayer(SelectLayer);
			obj->SetImage(_selectObjImage.second);
			obj->SetImageNumber(-1);
			obj->SetFileName(FileManager::GetInstance()->GetFileName(_selectObjImage.first));
			obj->SetObjectType(MapObject::ObjectType::kNoLife);
			obj->SetPath(_selectObjImage.first);
			obj->SetPos(
				static_cast<float>(GetMouse()->GetInfo().x) - static_cast<int>(ScrollManager::GetScrollX()),
				static_cast<float>(GetMouse()->GetInfo().y) - static_cast<int>(ScrollManager::GetScrollY()));
			obj->SetCX(_selectObjImage.second->GetWidth());
			obj->SetCY(_selectObjImage.second->GetHeight());

			_list[obj->GetLayer()].emplace_back(obj);
		}
		break;
	case MapManager::SelectState::kEtc:
		if (_selectEtcImage.second != nullptr)
		{
			if (_selectEtcImage.first.find("portal") != std::string::npos)
			{

			}
			else if (_selectEtcImage.first.find("rope") != std::string::npos)
			{
				std::cout << "로프" << std::endl;
			}
			else if (_selectEtcImage.first.find("ladder") != std::string::npos)
			{

			}
		}
		break;
	case MapManager::SelectState::kMob:
		if (_selectMonsterImage.second != nullptr)
		{
			MapObject* obj = new MapObject();

			obj->SetLayer(SelectLayer);
			obj->SetImage(_selectMonsterImage.second);
			obj->SetImageNumber(-2);
			obj->SetFileName(FileManager::GetInstance()->GetFileName(_selectMonsterImage.first));
			obj->SetObjectType(MapObject::ObjectType::kLife);
			obj->SetPath(_selectMonsterImage.first);
			obj->SetPos(
				static_cast<float>(GetMouse()->GetInfo().x) - static_cast<int>(ScrollManager::GetScrollX()),
				static_cast<float>(GetMouse()->GetInfo().y) - static_cast<int>(ScrollManager::GetScrollY()));
			obj->SetCX(_selectMonsterImage.second->GetWidth());
			obj->SetCY(_selectMonsterImage.second->GetHeight());

			_list[obj->GetLayer()].emplace_back(obj);
		}
		break;
	default:
		break;
	}
}

void MapManager::SelectTileImage(POINT& pt)
{
	int countX = 0;
	int countY = 0;
	for (auto tiles : _tileImages)
	{
		if (!strcmp(tiles.first.c_str(), _selectTileBegin->first.c_str()))
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
					_selectTileImage = tile.second;
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
}

void MapManager::SelectButtonImage(POINT& pt)
{
	int countX = 0;
	int countY = 0;
	for (auto tile : _buttons)
	{
		RECT rc{};
		int x = 90 + (90 * countX) + 1024;
		int y = 0;
		RECT tile_rc{
			x,
			y,
			x + static_cast<LONG>(tile.second->GetWidth()),
			y + static_cast<LONG>(tile.second->GetHeight()) };
		RECT mouse_rc{ pt.x, pt.y, pt.x + 1, pt.y + 1 };
		if (IntersectRect(&rc, &mouse_rc, &tile_rc))
		{
			if (!strcmp(tile.first.c_str(), "TileButton"))
			{
				_selectState = SelectState::kTile;
				if ((++_selectTileBegin) == _tileImages.end())
				{
					_selectTileBegin = _tileImages.begin();
				}
			}
			else if (!strcmp(tile.first.c_str(), "ObjButton"))
			{
				_selectState = SelectState::kObject;
			}
			else if (!strcmp(tile.first.c_str(), "EtcButton"))
			{
				_selectState = SelectState::kEtc;
			}
			else if (!strcmp(tile.first.c_str(), "MobButton"))
			{
				_selectState = SelectState::kMob;
			}
		}
		if (countX++ > 0 && countX % 4 == 0)
		{
			countX = 0;
			countY += 1;
		}
	}
}

void MapManager::SelectObjectImage(POINT& pt)
{
	int countX = 0;
	int countY = 0;
	for (auto tile : _objImages)
	{
		RECT rc{};
		RECT tile_rc{
			static_cast <int>(50 + (50 * countX) + 1024),
			static_cast <int>(50 + (50 * countY)),
			static_cast <int>(50 + (50 * countX) + 1024) +50,
			static_cast <int>(50 + (50 * countY)) +50 };
		RECT mouse_rc{ pt.x, pt.y, pt.x + 1, pt.y + 1 };
		if (IntersectRect(&rc, &mouse_rc, &tile_rc))
		{
			_selectObjImage = tile;
		}
		if (countX++ > 0 && countX % 8 == 0)
		{
			countX = 0;
			countY += 1;
		}
	}
}

void MapManager::SelectEtcImage(POINT& pt)
{
	int countX = 0;
	int countY = 0;
	for (auto tile : _etcImage)
	{
		RECT rc{};
		RECT tile_rc{
			static_cast <int>(100 + (100 * countX) + 1024),
			static_cast <int>(80 + (80 * countY)),
			static_cast <int>(100 + (100 * countX) + 1024) + tile.second->GetWidth(),
			static_cast <int>(80 + (80 * countY)) + 80 + tile.second->GetHeight() };
		RECT mouse_rc{ pt.x, pt.y, pt.x + 1, pt.y + 1 };
		if (IntersectRect(&rc, &mouse_rc, &tile_rc))
		{
			_selectEtcImage = tile;
		}
		if (countX++ > 0 && countX % 4 == 0)
		{
			countX = 0;
			countY += 1;
		}
	}
}

void MapManager::SelectMobImage(POINT& pt)
{

	int countX = 0;
	int countY = 0;
	for (auto tile : _monsterImage)
	{
		RECT rc{};
		RECT tile_rc{
			static_cast <int>(50 + (50 * countX) + 1024),
			static_cast <int>(50 + (50 * countY)),
			static_cast <int>(50 + (50 * countX) + 1024 + tile.second->GetWidth()) ,
			static_cast <int>(50 + (50 * countY)) + 50 + tile.second->GetHeight() };
		//image.second->GetWidth(), image.second->GetHeight()
		RECT mouse_rc{ pt.x, pt.y, pt.x + 1, pt.y + 1 };
		if (IntersectRect(&rc, &mouse_rc, &tile_rc))
		{
			_selectMonsterImage = tile;
		}
		if (countX++ > 0 && countX % 8 == 0)
		{
			countX = 0;
			countY += 1;
		}
	}
}

void MapManager::MouseUpdate(POINT& pt)
{
	switch (_selectState)
	{
	case MapManager::SelectState::kTile:
	{
		if (_selectTileImage.empty()) {
			return;
		}
		GetMouse()->SetCX(_selectTileImage[_selectCount]->GetWidth());
		GetMouse()->SetCY(_selectTileImage[_selectCount]->GetHeight());
		GetMouse()->UpdateRect();
		int countX = 0;
		int countY = 0;
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
	break;
	case MapManager::SelectState::kObject:
	{
		if (_selectObjImage.second!=nullptr)
		{
			GetMouse()->SetCX(_selectObjImage.second->GetWidth());
			GetMouse()->SetCY(_selectObjImage.second->GetHeight());
			GetMouse()->UpdateRect();
			int countX = 0;
			int countY = 0;
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
		break;
	}
	case MapManager::SelectState::kEtc:
		break;
	case MapManager::SelectState::kMob:
	{
		if (_selectMonsterImage.second != nullptr)
		{
			GetMouse()->SetCX(_selectMonsterImage.second->GetWidth());
			GetMouse()->SetCY(_selectMonsterImage.second->GetHeight());
			GetMouse()->UpdateRect();
			int countX = 0;
			int countY = 0;
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
		break;
	}
	default:
		break;
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
			MapObject::ObjectType objectState = obj->GetObjectType();
			WriteFile(hFile, &objectState, sizeof(objectState), &dwByte, nullptr);
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


	size_t ropeSize = _ladderRopes.size();
	WriteFile(hFile, &ropeSize, sizeof(ropeSize), &dwByte, nullptr);
	for (auto rope : _ladderRopes)
	{
		auto state = rope->GetState();
		WriteFile(hFile, &state, sizeof(state), &dwByte, nullptr);
		auto startPos = rope->GetStartPos();
		WriteFile(hFile, &startPos, sizeof(startPos), &dwByte, nullptr);
		auto endPos = rope->GetEndPos();
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
			MapObject::ObjectType objectState;
			check = ReadFile(hFile, &objectState, sizeof(objectState), &dwByte, nullptr);
			obj->SetObjectType(objectState);
			if (obj->GetImageNumber() == -1)
			{
				auto image = _objImages.find(obj->GetPath());
				obj->SetImage(image->second);
				_list[layer].push_back(obj);
			}
			else if (obj->GetImageNumber() == -2)
			{
				auto image = _monsterImage.find(obj->GetPath());
				obj->SetImage(image->second);
				_list[layer].push_back(obj);
			}
			else
			{
				std::string fullPath;
				fullPath.append(obj->GetPath()).append("\\").append(obj->GetFileName()).append(".").
					append(std::to_string(obj->GetImageNumber())).append(".bmp");
				auto image = _tileImages.find(obj->GetPath());
				auto file = image->second.find(obj->GetFileName());
				auto tile = file->second[obj->GetImageNumber()];
				obj->SetImage(tile);
				_list[layer].push_back(obj);
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
		_footholds.push_back(footHold);
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
		_ladderRopes.push_back(footHold);
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

	_tileImages.insert({ StringTools::WStringToString(name.c_str()), list });
	_selectTileBegin = _tileImages.begin();
}

void MapManager::TileRender(HDC hdc)
{
	int countX = 0;
	int countY = 0;
	for (auto tile : _selectTileBegin->second)
	{
		tile.second[0]->RenderBitmapImage(hdc,
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

void MapManager::FootHoldRender(HDC hdc)
{
	for (auto foot : _footholds)
	{
		foot->RenderFootHold(hdc);
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
			_objImages.insert(std::make_pair(StringTools::WStringToString(wpath.c_str()), image));
		}
	}
}

void MapManager::MapObjectRender(HDC hdc)
{
	int countX = 0;
	int countY = 0;
	for (auto image : _objImages)
	{
		image.second->RenderBitmapImage(hdc,
			static_cast <int>(50 + (50 * countX) + 1024),
			static_cast <int>(50 + (50 * countY)),
			50,
			50,
			image.second->GetWidth(), image.second->GetHeight());
		if (countX++ > 0 && countX % 8 == 0)
		{
			countX = 0;
			countY += 1;
		}
	}
}

void MapManager::EtcImageLoad()
{
	auto files = FileManager::GetInstance()->GetDirFileName(L"Client\\Etc\\");

	for (auto wpath : files)
	{
		auto path = StringTools::WStringToString(wpath.c_str());
		if (!_access(path.c_str(), 0))
		{
			MyBitmap* image = new MyBitmap;
			image->Insert_Bitmap(_hWnd, wpath.c_str());
			_etcImage.insert(std::make_pair(StringTools::WStringToString(wpath.c_str()), image));
		}
	}
}

void MapManager::EtcRender(HDC hdc)
{
	int countX = 0;
	int countY = 0;
	for (auto image : _etcImage)
	{
		image.second->RenderBitmapImage(hdc,
			static_cast <int>(100 + (100 * countX) + 1024),
			static_cast <int>(80 + (80 * countY)),
			image.second->GetWidth(),
			image.second->GetHeight(),
			image.second->GetWidth(), 
			image.second->GetHeight());
		if (countX++ > 0 && countX % 4 == 0)
		{
			countX = 0;
			countY += 1;
		}
	}
}

void MapManager::LadderRopeRender(HDC hdc)
{
	for (auto& data : _ladderRopes)
	{
		data->RenderFootHold(hdc);
	}
}

void MapManager::MonsterImageLoad()
{
	auto files = FileManager::GetInstance()->GetDirFileName(L"Client\\Mob\\");

	for (auto wpath : files)
	{
		auto path = StringTools::WStringToString(wpath.c_str());
		if (wpath.find(L".xml") != std::wstring::npos)
		{
			if (!_access(path.c_str(), 0))
			{
				// Client\\Mob\\0100100.img.xml
				auto name = FileManager::GetInstance()->GetFileName(path);
				auto  fullpath = wpath.substr(0, wpath.size() - 4);
				fullpath.append(L"\\stand.0.bmp");
				MyBitmap* image = new MyBitmap;
				image->Insert_Bitmap(_hWnd, fullpath.c_str());
				_monsterImage.insert(std::make_pair(StringTools::WStringToString(wpath.c_str()), image));
			}
		}
	}
}

void MapManager::MonsterRender(HDC hdc)
{
	int countX = 0;
	int countY = 0;
	for (auto image : _monsterImage)
	{
		image.second->RenderBitmapImage(hdc,
			static_cast <int>(50 + (50 * countX) + 1024),
			static_cast <int>(50 + (50 * countY)),
			50,
			50,
			image.second->GetWidth(), image.second->GetHeight());
		if (countX++ > 0 && countX % 8 == 0)
		{
			countX = 0;
			countY += 1;
		}
	}
}

void MapManager::ButtonLoad()
{
	MyBitmap* image = new MyBitmap;
	image->Insert_Bitmap(_hWnd, L"Client\\TileButton.bmp");
	_buttons.push_back(std::make_pair("TileButton", image));
	image = new MyBitmap;
	image->Insert_Bitmap(_hWnd, L"Client\\ObjButton.bmp");
	_buttons.push_back(std::make_pair("ObjButton", image));
	image = new MyBitmap;
	image->Insert_Bitmap(_hWnd, L"Client\\EtcButton.bmp");
	_buttons.push_back(std::make_pair("EtcButton", image));
	image = new MyBitmap;
	image->Insert_Bitmap(_hWnd, L"Client\\MobButton.bmp");
	_buttons.push_back(std::make_pair("MobButton", image));
}

void MapManager::ButtonRender(HDC hdc)
{
	int countX = 0;
	for (auto button : _buttons)
	{
		int x = 90 + (90 * countX) + 1024;
		int y = 0;
		button.second->RenderBitmapImage(hdc, x, y, button.second->GetWidth(), button.second->GetHeight());
		countX++;
	}
}

Mouse* MapManager::GetMouse()
{
	return _mouse;
}
