#pragma once

#include <sol/sol.hpp>

#include "../LOGGER/log.h"
#include "../ECS/Registry.h"
#include "../RENDERER/ESSENTIALS/Primitives.h"
#include "../RENDERER/CORE/Renderer.h"
#include "../RENDERER/CORE/Camera2D.h"
#include "../RESOURCES/AssetManager.h"

struct RendererBinder
{
    static void CreateRenderingBind(sol::state& lua, Registry& registry);
};

