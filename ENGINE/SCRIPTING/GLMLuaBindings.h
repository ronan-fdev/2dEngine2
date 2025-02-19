#pragma once

#include <sol/sol.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

struct GLMBindings
{
	static void CreateGLMBindings(sol::state& lua);
};