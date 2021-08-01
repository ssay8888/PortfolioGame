#pragma once
#include "../Managers/MapManager/map_object.h"
class Mouse : public MapObject
{
public:
	Mouse();
	~Mouse();
	void Ready_Map();
	void Update_Map();
	void Render_Map(HDC hDC);

private:
};

