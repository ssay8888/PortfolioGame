#pragma once
#include <list>
#include <map>
class MapObject;
class Mouse;
class FootHold;
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
	Gdiplus::Image* _selectImage;
	std::wstring _selectPath;
	std::list<MapObject*> _list[7];
	std::list<FootHold*> _foot[7];
	std::map<std::wstring, std::map<std::wstring, Gdiplus::Image*> > _images;
	Mouse* _mouse;
};

