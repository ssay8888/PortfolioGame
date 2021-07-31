#pragma once

class GameObject;
class Component
{
public:
	Component(GameObject* owner, int updateOrder = 100);
	virtual ~Component() = default;
	virtual void Update(float deltaTime) = 0;
	virtual void ProcessInput(const UINT8* keyState);

	int GetUpdateOrder() const;
protected:
	GameObject* _owner;
	int _updateOrder;
};

