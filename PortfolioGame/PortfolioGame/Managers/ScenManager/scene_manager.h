#pragma once
class Scene;
class LoadingScene;
class SceneManager
{

private:
	SceneManager();
	~SceneManager();
public:
	enum class  SceneState { kLoading, kLogin, kChannel, kCharacterSelect, kInGame, kEnd };

	static SceneManager* GetInstance()
	{
		static SceneManager instance;
		return &instance;
	}

public:
	void SceneChange(SceneState nextState);
	void UpdateSceneManager();
	void LateUpdateSceneManager();
	void RenderSceneManager(HDC hdc);
	void ReleaseSceneManager();

private:
	Scene* _scene;
	SceneState _curScene;
	SceneState _nextScene;
};

