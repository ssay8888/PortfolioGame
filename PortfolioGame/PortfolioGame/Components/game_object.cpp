#include "../pch.h"
#include "game_object.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::Update(const float deltaTime)
{
	if (_state == kActive)
	{
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);
	}
}

void GameObject::UpdateComponents(const float deltaTime)
{
}

void GameObject::UpdateActor(const float deltaTime)
{
}
