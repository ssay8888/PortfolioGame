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
	static MapManager* GetInstance()
	{
		static MapManager instance;
		return &instance;
	}

	void Ready_Map();
	void Update_Map();
	void Render_Map(HDC hDC);

	void AddListObject();
	void SelectImage(POINT& pt);
	void MouseUpdate(POINT& pt);

	void SaveData();
	void LoadData();

	void TileLoad(std::wstring name);
	void MapObjectLoad();

	void ButtonLoad();
	void ButtonRender(HDC hdc);


	Mouse* GetMouse();

private:
	std::vector<MyBitmap*> _selectImage;
	uint32_t _selectCount;
	std::string _selectFileName;
	std::string _selectPath;
	std::list<MapObject*> _list[7];
	std::list<FootHold*> _footholds;
	std::map<std::string, std::map<std::string, std::vector<MyBitmap*>> > _tileImages;
	std::vector<MyBitmap*> _objImages;
	std::vector<MyBitmap*> _buttons;
	POINT _mapSize;
	Mouse* _mouse;
};

