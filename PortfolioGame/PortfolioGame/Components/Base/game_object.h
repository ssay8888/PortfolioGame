#pragma once
class MyBitmap;
class GameObject
{
public:
	enum class State
	{
		kActive,
		kPaused,
		kDead
	};
	GameObject(uint8_t layer);
	virtual ~GameObject();

	void UpdateRectGameObject();
	void DoReadyGame();
	GameObject::State DoUpdateObject(const float deltaTime);
	void DoRenderGameObject(HDC hdc);
	GameObject::State DoLateUpdateGameObject();

	std::string GetPath();
	std::string GetFileName();
	uint8_t GetLayer() const;
	GameObject::State GetState() const;
	Info GetInfo() const;
	RECT GetRect() const;
	MyBitmap* GetImage();
	float GetSpeed() const;
	uint32_t GetImageNumber() const;

	void SetPath(std::string path);
	void SetFileName(std::string fileName);
	void SetLayer(uint8_t layer);
	void SetState(GameObject::State state);
	void SetInfo(Info info);
	void SetRect(RECT rect);
	void SetImage(MyBitmap* image);
	void SetImageNumber(uint32_t number);

private:
	virtual int ReadyGameObject() = 0;
	virtual void UpdateGameObject(const float deltaTime) = 0;
	virtual void RenderGameObject(HDC hdc) = 0;
	virtual void LateUpdateGameObject() = 0;

protected:
	std::string _path;
	std::string _fileName;
	uint8_t _layer;
	uint32_t _imageNumber;
	State _state;
	Info _info;
	RECT _rect;
	float _speed;
	MyBitmap* _image;

	HDC _memDC;
	HBITMAP _bitmap;
	HBITMAP _old_bitmap;
};

