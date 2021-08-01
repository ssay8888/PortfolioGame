#include "../../pch.h"
#include "../../Components/Base/game_object.h"
#include "object_manager.h"

void ObjectManager::AddGameObject(GameObject* object)
{
	auto layer = object->GetLayer();
	
	for (auto iter = _listGameObject.begin(); iter != _listGameObject.end(); ++iter)
	{
		if (layer < (*iter)->GetLayer())
		{
			_listGameObject.insert(iter, object);
			return;
		}
	}

	_listGameObject.emplace_back(object);
}

void ObjectManager::UpdateGameObjectManager(const float deltaTime)
{
	for (auto iter = _listGameObject.begin(); iter != _listGameObject.end();)
	{
		GameObject::State iEvent = (*iter)->DoUpdateObject(deltaTime);
		if (GameObject::State::kDead == iEvent)
		{
			if ((*iter))
			{
				delete (*iter);
				(*iter) = nullptr;
			}
			iter = _listGameObject.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void ObjectManager::RenderGameObjectManager(HDC hdc)
{
	for (auto& pObject : _listGameObject)
	{
		pObject->DoRenderGameObject(hdc);
	}
}

void ObjectManager::ReleaseGameObjectManager()
{
}
