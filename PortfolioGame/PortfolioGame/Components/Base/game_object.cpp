#include "../../pch.h"
#include "game_object.h"
#include "../../../Common/Managers/BitmapManager/my_bitmap.h"

GameObject::GameObject(uint8_t layer) :
	_layer(layer),
	_info({}),
	_rect({}),
	_speed(0),
	_state(GameObject::State::kActive),
	_image(nullptr)
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
		UpdateGameObject(deltaTime);
	}
	return this->_state;
}

void GameObject::DoRenderGameObject(HDC hdc)
{
	RenderGameObject(hdc);
}

GameObject::State GameObject::DoLateUpdateGameObject()
{
	if (this->_state == GameObject::State::kActive)
	{
		LateUpdateGameObject();
	}
	return this->_state;
}

std::string GameObject::GetPath()
{
	return _path;
}

std::string GameObject::GetFileName()
{
	return _fileName;
}

uint8_t GameObject::GetLayer() const
{
	return _layer;
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

MyBitmap* GameObject::GetImage()
{
	return _image;
}

float GameObject::GetSpeed() const
{
	return _speed;
}

uint32_t GameObject::GetImageNumber() const
{
	return _imageNumber;
}

void GameObject::SetPath(std::string path)
{
	_path = path;
}

void GameObject::SetFileName(std::string fileName)
{
	_fileName = fileName;
}

void GameObject::SetLayer(uint8_t layer)
{
	_layer = layer;
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

void GameObject::SetImage(MyBitmap* image)
{
	_image = image;
}

void GameObject::SetImageNumber(uint32_t number)
{
	_imageNumber = number;
}

void GameObject::UpdateGameObject(const float deltaTime)
{
}
