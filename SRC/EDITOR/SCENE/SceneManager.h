#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

class SceneObject;

#include "ENGINE/LOGGER/log.h"

#include "ENGINE/UTILITIES/2dEngineUtilities.h"

#define SCENE_MANAGER() SceneManager::GetInstance()

class SceneManager
{
private:
	std::map<std::string, std::shared_ptr<SceneObject>> m_mapScenes;
	std::string m_sCurrentScene{ "" };
private:
	SceneManager() = default;
	~SceneManager() = default;
	SceneManager(const SceneManager&) = delete;//Deletes the copying of the class
	SceneManager& operator=(const SceneManager&) = delete;//Deletes the assignment of the class.
public:
	static SceneManager& GetInstance();

	bool AddScene(const std::string& sSceneName);
	std::shared_ptr<SceneObject> GetScene(const std::string& sSceneName);
	std::shared_ptr<SceneObject> GetCurrentScene();
	std::vector<std::string> GetSceneNames() const;

	inline void SetCurrentScene(const std::string& sSceneName) { m_sCurrentScene = sSceneName; }
	inline const std::string& GetCurrentSceneName() const { return m_sCurrentScene; }
};

