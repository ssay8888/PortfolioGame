#pragma once
class GameObject
{
public:
	enum State
	{
		kActive,
		kPaused,
		kDead
	};
	GameObject();
	virtual ~GameObject();

	void Update(const float deltaTime);
	void UpdateComponents(const float deltaTime);
	virtual void UpdateActor(const float deltaTime);

private:
	State _state;
	ObjectInfo _info;
};

