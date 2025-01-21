#pragma once

#include "../ECS/Registry.h"
#include "../ECS/COMPONENTS/ScriptComponent.h"
#include "../ECS/COMPONENTS/TransformComponent.h"
#include "../ECS/COMPONENTS/SpriteComponent.h"
#include "../ECS/Entity.h"
#include "../LOGGER/log.h"

#include <sol/sol.hpp>

class ScriptingSystem
{
public:
	ScriptingSystem(Registry& registry);
	~ScriptingSystem() = default;
	bool LoadMainScript(sol::state& lua);
	void Update();
	void Render();

	static void RegisterLuaBindings(sol::state& lua, Registry& registry);

private:
	Registry& m_Registry;
	bool m_bMainLoaded;
};
