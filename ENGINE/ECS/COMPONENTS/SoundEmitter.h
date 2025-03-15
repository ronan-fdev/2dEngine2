#pragma once

#include <string>
#include <memory>
#include <sol/sol.hpp>
#include <entt/entt.hpp>
#include "../Registry.h"
#include "../../SOUND/AudioEvent.h"
#include "../../SYSTEMS/SoundSystem.h"

struct SoundEmitter
{
	std::string eventPath;
	std::shared_ptr<AudioEvent> audioEvent;

	static void CreateLuaSoundEmitterBind(sol::state& lua, Registry& registry);
};
