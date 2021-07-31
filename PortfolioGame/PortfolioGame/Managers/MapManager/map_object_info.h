#pragma once
#include <string>
class MapObjectInfo 
{
public:
	MapObjectInfo();
	~MapObjectInfo();

	void SetName(const std::string name);
	std::string GetName() const;

	void SetX(const uint32_t x);
	void SetY(const uint32_t y);
	void SetLayer(const uint32_t layer);

	uint32_t GetX() const;
	uint32_t GetY() const;
	uint8_t GetLayer() const;
private:
	std::string _name;
	uint32_t _x;
	uint32_t _y;
	uint8_t _layer;
};