#include "../../pch.h"
#include "scene_manager.h"
#include "LoadingScene/loading_scene.h"


SceneManager::SceneManager()
	:_curScene(SceneState::kEnd)
	,_nextScene(SceneState::kEnd)
	,_scene(nullptr)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::SceneChange(SceneState nextState)
{
	_nextScene = nextState;
	if (_curScene != _nextScene)
	{
		delete _scene;
		_scene = nullptr;
		switch (_nextScene)
		{
		case SceneManager::SceneState::kLoading:
			_scene = new LoadingScene();
			break;
		case SceneManager::SceneState::kLogin:
			//_scene = new LoginScene();
			break;
		case SceneManager::SceneState::kChannel:
			break;
		case SceneManager::SceneState::kCharacterSelect:
			break;
		case SceneManager::SceneState::kInGame:
			break;
		case SceneManager::SceneState::kEnd:
			break;
		default:
			break;
		}
		_curScene = _nextScene;
	}

}

void SceneManager::UpdateSceneManager()
{
	_scene->UpdateScene();
}

void SceneManager::LateUpdateSceneManager()
{
	_scene->LateUpdateScene();
}

void SceneManager::RenderSceneManager(HDC hdc)
{
	_scene->RenderScene(hdc);

}

void SceneManager::ReleaseSceneManager()
{
}
