#pragma once

#include "../ECS/Registry.h"
#include "../ECS/COMPONENTS/ScriptComponent.h"
#include "../ECS/Entity.h"
#include "../LOGGER/log.h"

#include <sol/sol.hpp>

class ScriptingSystem
{
private:
	Registry& m_Registry;
	bool m_bMainLoaded;
public:
	ScriptingSystem(Registry& registry);
	~ScriptingSystem() = default;
	bool LoadMainScript(sol::state& lua);
	void Update();
	void Render();
};
