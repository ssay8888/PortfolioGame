#include "../../pch.h"
#include "component.h"
#include "game_object.h"

Component::Component(GameObject* owner, int updateOrder) :
	_owner(owner),
	_updateOrder(updateOrder)
{
}

void Component::ProcessInput(const UINT8* keyState)
{
}

inline int Component::GetUpdateOrder() const 
{
	return _updateOrder; 
}
