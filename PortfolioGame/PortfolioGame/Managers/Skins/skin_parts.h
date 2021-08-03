#pragma once
class MyBitmap;
class SkinFrame;
class SkinParts
{
public:
	SkinParts() = default;
	~SkinParts() = default;

	std::string GetName() const;
	void SetName(std::string name);

	void SetMap(std::map<std::string, POINT> map);
	std::map<std::string, POINT> GetMap() const;

	void InsertMap(std::string key, POINT pos);
	POINT FindMap(std::string key);
	std::map<std::string, POINT>* GetMaps();

	POINT GetOrigin() const;
	void SetOrigin(POINT pos);
	void SetOriginX(LONG x);
	void SetOriginY(LONG y);

	std::string GetZ() const;
	void SetZ(std::string z);

	uint16_t GetDelay() const;
	void SetDelay(uint16_t delay);

	std::string GetUol() const;
	void SetUol(std::string uol);

	MyBitmap* GetBitmap() const;
	void SetBitmap(MyBitmap* bitmap);

	uint16_t GetPosition() const;
	void SetPosition(uint16_t pos);

	void SetPartner(SkinFrame* partner);
	SkinFrame* GetPartner() const;
private:
	std::string _name;
	std::map<std::string, POINT> _map;
	POINT _origin;
	std::string _z;
	uint16_t _delay;
	std::string _uol;
	MyBitmap* _image;
	uint16_t _position;


	SkinFrame* _partner;
};

