#pragma once
class MyBitmap;
class MapObject
{
public:
	enum class ObjectType { kNoLife, kLife, kNpc };
	MapObject();
	~MapObject();



	void UpdateRect();

	MyBitmap* GetImage() const;
	uint32_t GetImageNumber() const;
	Info GetInfo() const;
	RECT GetRect() const;
	uint32_t GetLayer() const;
	std::string GetFileName() const;
	std::string GetPath() const;
	ObjectType GetObjectType() const; 


	void SetFileName(std::string path);
	void SetPath(std::string path);
	void SetImage(MyBitmap* image);
	void SetImageNumber(uint32_t number);
	void SetInfo(Info info);
	void SetRect(RECT rect);
	void SetPos(float fX, float fY);
	void SetCY(int x);
	void SetCX(int y);
	void SetLayer(uint32_t layer);
	void SetObjectType(const ObjectType type);

private:
	MyBitmap* _image;
	uint32_t _imageNumber;
	std::string _path;
	std::string _fileName;
	Info _info;
	RECT _rect;
	uint32_t _layer;
	ObjectType _state;
};

