#pragma once
class MyBitmap;
class MapObject
{
public:
	MapObject();
	~MapObject();



	void UpdateRect();

	MyBitmap* GetImage() const;
	Info GetInfo() const;
	RECT GetRect() const;
	uint32_t GetLayer() const;
	std::string GetFileName() const;
	std::string GetPath() const;


	void SetFileName(std::string path);
	void SetPath(std::string path);
	void SetImage(MyBitmap* image);
	void SetInfo(Info info);
	void SetRect(RECT rect);
	void SetPos(float fX, float fY);
	void SetCY(int x);
	void SetCX(int y);
	void SetLayer(uint32_t layer);

private:
	MyBitmap* _image;
	std::string _path;
	std::string _fileName;
	Info _info;
	RECT _rect;
	uint32_t _layer;
};

