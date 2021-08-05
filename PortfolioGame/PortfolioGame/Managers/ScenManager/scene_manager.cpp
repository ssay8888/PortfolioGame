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
		switch (_nextScene)
		{
		case SceneManager::SceneState::kLoading:
			_scene = std::make_shared<Scene*>(new LoadingScene());
			break;
		case SceneManager::SceneState::kLogin:
			_scene = std::make_shared<Scene*>(new LoginUiScene());
			break;
		case SceneManager::SceneState::kChannel:
			break;
		case SceneManager::SceneState::kCharacterSelect:
			break;
		case SceneManager::SceneState::kInGame:
			_scene = std::make_shared<Scene*>(new InGameScene());
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
	(*_scene.get())->UpdateScene();
}

void SceneManager::LateUpdateSceneManager()
{
	(*_scene.get())->LateUpdateScene();
}

void SceneManager::RenderSceneManager(HDC hdc)
{
	(*_scene.get())->RenderScene(hdc);

}

void SceneManager::ReleaseSceneManager()
{
}
