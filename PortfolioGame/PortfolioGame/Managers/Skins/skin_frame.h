#pragma once
class SkinFrame
{
public:
	SkinFrame();
	~SkinFrame();
	void AddMap(const std::string name, const ObjectPos pos);
	void AddMap(std::pair<std::string, ObjectPos> map);

	void SetMap(std::map<std::string, ObjectPos> map);
	std::map<std::string, ObjectPos>& GetMap();
	const ObjectPos& GetMapItem(const std::string name);
	size_t GetMapSize() const;

	void SetName(const std::string name);
	const std::string GetName() const;

	void SetPath(const std::string path);
	const std::string GetPath();

	void SetUol(const std::string uol);
	std::string GetUol() const;
	SkinFrame* FindUolFrame();

	void SetUolFrame(SkinFrame* uol);
	SkinFrame* GetUolFrame();

	void SetZ(const std::string z);
	std::string GetZ();

	void SetPosition(const uint16_t z);
	uint16_t GetPosition();

	void SetOrigin(const ObjectPos pos);
	void SetOriginX(float x);
	void SetOriginY(float y);
	ObjectPos GetOrigin();

	void SetWidth(const uint32_t width);
	uint32_t GetWidth();

	void SetHeight(const uint32_t height);
	uint32_t GetHeight();

	void SetImage(Gdiplus::Image* image);
	Gdiplus::Image* GetImage();
private:
	std::map<std::string, ObjectPos> _map;
	std::string _name;
	std::string _path;
	std::string _uol;
	SkinFrame* _uolFrame;
	std::string _z;
	uint16_t _position;
	ObjectPos _origin;
	uint32_t _width;
	uint32_t _height;
	Gdiplus::Image* _image;
};

