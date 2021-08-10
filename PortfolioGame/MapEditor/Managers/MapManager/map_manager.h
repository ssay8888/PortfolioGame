#pragma once
#include <list>
#include <map>
class MapObject;
class Mouse;
class FootHold;
class MyBitmap;
class MapManager
{
private:
	MapManager();
	~MapManager() = default;
public:
	enum class SelectState { kTile, kObject, kEtc, kMob, kEnd};
	static MapManager* GetInstance()
	{
		static MapManager instance;
		return &instance;
	}

	void Ready_Map();
	void Update_Map();
	void Render_Map(HDC hDC);

	void AddListObject();
	void DeleteTile(POINT& pt);
	void SelectTileImage(POINT& pt);
	void SelectButtonImage(POINT& pt);
	void SelectObjectImage(POINT& pt);
	void SelectEtcImage(POINT& pt);
	void SelectMobImage(POINT& pt);
	void MouseUpdate(POINT& pt);

	void SaveData();
	void LoadData();

	void TileLoad(std::wstring name);
	void TileRender(HDC hdc);
	void FootHoldRender(HDC hdc);

	void MapObjectImageLoad();
	void MapObjectRender(HDC hdc);

	void EtcImageLoad();
	void EtcRender(HDC hdc);
	void LadderRopeRender(HDC hdc);

	void MonsterImageLoad();
	void MonsterRender(HDC hdc);

	void ButtonLoad();
	void ButtonRender(HDC hdc);


	Mouse* GetMouse();

private:
	std::list<MapObject*> _list[7];
	std::list<FootHold*> _footholds;
	std::list<FootHold*> _ladderRopes;

	SelectState _selectState;

	std::vector<MyBitmap*> _selectTileImage;
	std::string _selectFileName;
	std::string _selectPath;
	uint32_t _selectCount;
	std::map<std::string, std::map<std::string, std::vector<MyBitmap*>>> _tileImages;
	std::map<std::string, std::map<std::string, std::vector<MyBitmap*>>>::iterator _selectTileBegin;

	std::pair<std::string, MyBitmap*> _selectObjImage;
	std::map<std::string, MyBitmap*> _objImages;


	std::pair<std::string, MyBitmap*> _selectEtcImage;
	std::map<std::string, MyBitmap*> _etcImage;

	std::pair<std::string, MyBitmap*> _selectMonsterImage;
	std::map<std::string, MyBitmap*> _monsterImage;


	std::vector<std::pair<std::string, MyBitmap*>> _buttons;

	ObjectPos _mapSize;
	Mouse* _mouse;
	
	bool _is_altkey;
};

