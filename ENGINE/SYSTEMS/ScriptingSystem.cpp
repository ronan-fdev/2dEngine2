#include "ScriptingSystem.h"

ScriptingSystem::ScriptingSystem(Registry& registry)
	:
	m_Registry(registry), m_bMainLoaded{ false }
{
}

bool ScriptingSystem::LoadMainScript(sol::state& lua)
{
	try
	{
		auto result = lua.safe_script_file("ASSETS/SCRIPTS/main.lua");
	}
	catch (const sol::error& err)
	{
		LOG_ERROR("Error loading the main lua script: {0}", err.what());
		return false;
	}
	sol::table main_lua = lua["main"];
	sol::optional<sol::table> bUpdateExists = main_lua[1];
	if (bUpdateExists == sol::nullopt)
	{
		LOG_ERROR("There is no update function in main.lua");
		return false;
	}
	sol::table update_script = main_lua[1];
	sol::function update = update_script["update"];

	sol::optional<sol::table> bRenderExists = main_lua[2];
	if (bRenderExists == sol::nullopt)
	{
		LOG_ERROR("There is no render function in main.lua");
		return false;
	}
	sol::table render_script = main_lua[2];
	sol::function render = render_script["render"];
	Entity mainLuaScript{ m_Registry, "main_script", "" };
	mainLuaScript.AddComponent<ScriptComponent>(
		ScriptComponent{
			.update = update,
			.render = render
		}
	);
	m_bMainLoaded = true;
	return true;
}

void ScriptingSystem::Update()
{
	if (!m_bMainLoaded)
	{
		LOG_ERROR("Main lua script has not been loaded!");
		return;
	}
	auto view = m_Registry.GetRegistry().view<ScriptComponent>();
	for (const auto& entity : view)
	{
		Entity ent{ m_Registry, entity };
		if (ent.GetName() != "main_script")
			continue;
		auto& script = ent.GetComponent<ScriptComponent>();
		auto error = script.update(entity);
		if (!error.valid())
		{
			sol::error err = error;
			LOG_ERROR("Error running the Update script: {0}", err.what());
		}
	}
}

void ScriptingSystem::Render()
{
	if (!m_bMainLoaded)
	{
		LOG_ERROR("Main lua script has not been loaded!");
		return;
	}
	auto view = m_Registry.GetRegistry().view<ScriptComponent>();
	for (const auto& entity : view)
	{
		Entity ent{ m_Registry, entity };
		if (ent.GetName() != "main_script")
			continue;
		auto& script = ent.GetComponent<ScriptComponent>();
		auto error = script.render(entity);
		if (!error.valid())
		{
			sol::error err = error;
			LOG_ERROR("Error running the Render script: {0}", err.what());
		}
	}
}
