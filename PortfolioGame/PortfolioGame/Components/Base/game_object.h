#pragma once
class Component;
class GameObject
{
public:
	enum class State
	{
		kActive,
		kPaused,
		kDead
	};
	GameObject();
	virtual ~GameObject();

	void UpdateRectGameObject();
	void DoReadyGame();
	GameObject::State DoUpdateObject(const float deltaTime);
	void DoRenderGameObject(HDC hdc);
	void DoLateUpdateGameObject();
	void UpdateComponents(const float deltaTime);

	GameObject::State GetState() const;
	Info GetInfo() const;
	RECT GetRect() const;

	void SetState(GameObject::State state);
	void SetInfo(Info info);
	void SetRect(RECT rect);

	std::list<Component*> GetComponents();
	void AddComponents(Component* item);


private:
	virtual int ReadyGameObject() = 0;
	virtual void UpdateActor(const float deltaTime) = 0;
	virtual void RenderGameObject(HDC hdc) = 0;
	virtual void LateUpdateGameObject() = 0;

protected:
	State _state;
	Info _info;
	RECT _rect;
	float _speed;
	std::list<Component*> _components;
};

