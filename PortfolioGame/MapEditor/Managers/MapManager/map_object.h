#pragma once
class MapObject
{
public:
	MapObject();
	~MapObject();



	void UpdateRect();

	Gdiplus::Image* GetImage() const;
	Info GetInfo() const;
	uint32_t GetLayer() const;

	void SetImage(Gdiplus::Image* image);
	void SetInfo(Info info);
	void SetPos(float fX, float fY);
	void SetCY(int x);
	void SetCX(int y);
	void SetLayer(uint32_t layer);

private:
	Gdiplus::Image* _image;
	Info _info;
	RECT _rect;
	uint32_t _layer;
};

