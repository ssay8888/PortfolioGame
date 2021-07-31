#include "../../pch.h"
#include "../../Components/Base/game_object.h"
#include "object_manager.h"

void ObjectManager::AddGameObject(ObjectType::ObjectType id, GameObject* object)
{
	_listGameObject[static_cast<uint32_t>(id)].emplace_back(object);
}

void ObjectManager::UpdateGameObjectManager(const float deltaTime)
{
	for (int i = 0; i < static_cast<uint16_t>(ObjectType::ObjectType::kEnd); ++i)
	{
		for (auto iter = _listGameObject[i].begin(); iter != _listGameObject[i].end();)
		{
			GameObject::State iEvent = (*iter)->DoUpdateObject(deltaTime);
			if (GameObject::State::kDead == iEvent)
			{
				if ((*iter))
				{
					delete (*iter);
					(*iter) = nullptr;
				}
				iter = _listGameObject[i].erase(iter);
			}
			else
				++iter;
		}

	}
}

void ObjectManager::RenderGameObjectManager(HDC hdc)
{
	for (int i = 0; i < static_cast<uint16_t>(ObjectType::ObjectType::kEnd); ++i)
	{
		for (auto& pObject : _listGameObject[i])
			pObject->DoRenderGameObject(hdc);
	}
}

void ObjectManager::ReleaseGameObjectManager()
{
}
