#pragma once
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
	void DoLateUpdateGameObject();

	std::string GetPath();
	std::string GetFileName();
	uint8_t GetLayer() const;
	GameObject::State GetState() const;
	Info GetInfo() const;
	RECT GetRect() const;

	void SetPath(std::string path);
	void SetFileName(std::string fileName);
	void SetLayer(uint8_t layer);
	void SetState(GameObject::State state);
	void SetInfo(Info info);
	void SetRect(RECT rect);

private:
	virtual int ReadyGameObject() = 0;
	virtual void UpdateGameObject(const float deltaTime) = 0;
	virtual void RenderGameObject(HDC hdc) = 0;
	virtual void LateUpdateGameObject() = 0;

protected:
	std::string _path;
	std::string _fileName;
	uint8_t _layer;
	State _state;
	Info _info;
	RECT _rect;
	float _speed;
};

