#pragma once

class Actor;
class Component
{
public:
	Component(class Actor* owner, int updateOrder = 100);
	virtual ~Component() = default;
	virtual void Update(float deltaTime) = 0;
	virtual void ProcessInput(const UINT8* keyState);

	int GetUpdateOrder() const { return mUpdateOrder; }
protected:
	Actor* mOwner;
	int mUpdateOrder;
};

