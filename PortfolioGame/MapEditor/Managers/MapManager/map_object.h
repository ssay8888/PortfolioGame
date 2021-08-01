#pragma once
class MapObject
{
public:
	MapObject();
	~MapObject();



	void UpdateRect();

	Gdiplus::Image* GetImage() const;
	Info GetInfo() const;
	RECT GetRect() const;
	uint32_t GetLayer() const;
	std::wstring GetPath() const;


	void SetPath(std::wstring path);
	void SetImage(Gdiplus::Image* image);
	void SetInfo(Info info);
	void SetRect(RECT rect);
	void SetPos(float fX, float fY);
	void SetCY(int x);
	void SetCX(int y);
	void SetLayer(uint32_t layer);

private:
	Gdiplus::Image* _image;
	std::wstring _path;
	Info _info;
	RECT _rect;
	uint32_t _layer;
};

