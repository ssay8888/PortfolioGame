#pragma once
class MyBitmap;
class SkinFrame;
class SkinParts
{
public:
	SkinParts();
	~SkinParts() = default;

	std::string GetName() const;
	void SetName(std::string name);

	void SetMap(std::unordered_map<std::string, ObjectPos> map);
	std::unordered_map<std::string, ObjectPos> GetMap() const;

	void InsertMap(std::string key, ObjectPos pos);
	ObjectPos FindMap(std::string key);
	std::unordered_map<std::string, ObjectPos>* GetMaps();

	ObjectPos GetOrigin() const;
	void SetOrigin(ObjectPos pos);
	void SetOriginX(float x);
	void SetOriginY(float y);

	std::string GetZ() const;
	void SetZ(std::string z);

	uint16_t GetDelay() const;
	void SetDelay(uint16_t delay);

	std::string GetUol() const;
	void SetUol(std::string uol);

	MyBitmap* GetBitmap() const;
	void SetBitmap(MyBitmap* bitmap);

	void SetPartner(SkinFrame* partner);
	SkinFrame* GetPartner() const;

	void SetPosition(uint16_t position);
	uint16_t GetPosition() const;
private:
	std::string _name;
	std::unordered_map<std::string, ObjectPos> _map;
	ObjectPos _origin;
	std::string _z;
	uint16_t _delay;
	std::string _uol;
	MyBitmap* _image;
	uint16_t _position;


	SkinFrame* _partner;
};

