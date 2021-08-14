#include "../../pch.h"
#include "scene_manager.h"
#include "LoadingScene/loading_scene.h"
#include "LoginUiScene/login_ui_scene.h"
#include "InGameScene/in_game_scene.h"

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

		switch (_nextScene)
		{
		case SceneManager::SceneState::kLoading:
			_scene = new LoadingScene();
			break;
		case SceneManager::SceneState::kLogin:
			_scene = new LoginUiScene();
			break;
		case SceneManager::SceneState::kChannel:
			break;
		case SceneManager::SceneState::kCharacterSelect:
			break;
		case SceneManager::SceneState::kInGame:
			_scene = new InGameScene();
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
