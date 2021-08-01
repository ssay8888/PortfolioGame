#pragma once
class GameObject;
class ObjectManager
{
	ObjectManager() = default;
	~ObjectManager() = default;
public:
	static ObjectManager* Get_Instance()
	{
		static ObjectManager instance;
		return &instance;
	}

	void AddGameObject(GameObject* object);

	void UpdateGameObjectManager(const float deltaTime);
	void RenderGameObjectManager(HDC hDC);
	void ReleaseGameObjectManager();
private:
	std::list<GameObject*> _listGameObject;
};

