#include "SceneManager.h"

#include "SceneObject.h"

SceneManager& SceneManager::GetInstance()
{
	static SceneManager instance{};//Since it is static it will be created only one time and same instance will be returned everytime.
	return instance;
}

bool SceneManager::AddScene(const std::string& sSceneName)
{
	if (m_mapScenes.contains(sSceneName))
	{
		LOG_ERROR("Failed to add new scene object - [{}] already exists", sSceneName);
		return false;
	}

	auto result = m_mapScenes.emplace(sSceneName, std::move(std::make_shared<SceneObject>(sSceneName)));
	return result.second;
}

std::shared_ptr<SceneObject> SceneManager::GetScene(const std::string& sSceneName)
{
	auto sceneItr = m_mapScenes.find(sSceneName);
	if (sceneItr == m_mapScenes.end())
	{
		LOG_ERROR("Failed to get the scene object - [{}] does not exit", sSceneName);
		return nullptr;
	}

	return sceneItr->second;
}

std::shared_ptr<SceneObject> SceneManager::GetCurrentScene()
{
	if (m_sCurrentScene.empty())
	{
		return nullptr;
	}
	auto sceneItr = m_mapScenes.find(m_sCurrentScene);
	if (sceneItr == m_mapScenes.end())
	{
		LOG_ERROR("Failed to get the scene object - [{}] does not exit", m_sCurrentScene);
		return nullptr;
	}

	return sceneItr->second;
}

std::vector<std::string> SceneManager::GetSceneNames() const
{
	return GetKeys(m_mapScenes);
}
