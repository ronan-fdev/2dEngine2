#pragma once
#include <sol/sol.hpp>
#include <entt/entt.hpp>
#include "../../SYSTEMS/SoundSystem.h"

struct SoundListener
{
	bool isActive = false;
	static void CreateLuaSoundListenerBind(sol::state& lua);
};