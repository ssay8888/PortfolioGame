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


	Mouse* GetMouse();

private:
	MyBitmap* _selectImage;
	std::string _selectFileName;
	std::string _selectPath;
	std::list<MapObject*> _list[7];
	std::list<FootHold*> _footholds;
	std::map<std::string, std::map<std::string, MyBitmap*> > _images;
	Mouse* _mouse;
};

