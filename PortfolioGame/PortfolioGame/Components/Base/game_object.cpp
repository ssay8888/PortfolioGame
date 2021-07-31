#include "../../pch.h"
#include "game_object.h"
#include "component.h"

GameObject::GameObject() :
	_info({}),
	_rect({}),
	_speed(0),
	_state(GameObject::State::kActive)
{
}

GameObject::~GameObject()
{
}

void GameObject::UpdateRectGameObject()
{
	_rect.left = LONG(_info.x - (_info.cx >> 1));
	_rect.top = static_cast<LONG>(_info.y - (_info.cy * 0.5f));
	_rect.right = LONG(_info.x + (_info.cx >> 1));
	_rect.bottom = static_cast<LONG>(_info.y + (_info.cy * 0.5f));
}

void GameObject::DoReadyGame()
{
	ReadyGameObject();
}

GameObject::State GameObject::DoUpdateObject(const float deltaTime)
{
	if (this->_state == GameObject::State::kActive)
	{
		UpdateComponents(deltaTime);
		UpdateGameObject(deltaTime);
	}
	return this->_state;
}

void GameObject::DoRenderGameObject(HDC hdc)
{
	RenderGameObject(hdc);
}

void GameObject::DoLateUpdateGameObject()
{
	LateUpdateGameObject();
}

void GameObject::UpdateComponents(const float deltaTime)
{
}

GameObject::State GameObject::GetState() const
{
	return _state;
}

Info GameObject::GetInfo() const
{
	return _info;
}

RECT GameObject::GetRect() const
{
	return _rect;
}

void GameObject::SetState(GameObject::State state)
{
	_state = state;
}

void GameObject::SetInfo(Info info)
{
	_info = info;
}

void GameObject::SetRect(RECT rect)
{
	_rect = rect;
}

std::list<Component*> GameObject::GetComponents()
{
	return _components;
}

void GameObject::AddComponents(Component* item)
{
	_components.emplace_back(item);
}

void GameObject::UpdateGameObject(const float deltaTime)
{
}
