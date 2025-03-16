#pragma once

#include <sol/sol.hpp>

#include "../ECS/Registry.h"
#include "../RESOURCES/AssetManager.h"
#include "../SOUNDS/MUSICPLAYER/MusicPlayer.h"
#include "../LOGGER/log.h"

class Registry;

struct SoundBinder
{
	static void CreateSoundBind(sol::state& lua, Registry& registry);
};