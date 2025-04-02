#pragma once

#include <sol/sol.hpp>
#include <entt/entt.hpp>
#include "../../WINDOW/Window.h"

struct AnimationComponent
{
	int numFrames{ 1 }, frameRate{ 1 }, frameOffset{ 0 }, currentFrame{ 0 };
	int startTime{ static_cast<int>(glfwGetTime()) };
	bool bVertical{ false }, bLooped{ false };
	static void CreateAnimationLuaBind(sol::state& lua);
};
