#pragma once
#include <glm/glm.hpp>
#include <sol/sol.hpp>
#include <entt/entt.hpp>

struct CircleColliderComponent
{
	float radius{ 0.f };
	glm::vec2 offset{ glm::vec2{0} };
	bool bColliding{ false };
	static void CreateLuaCircleColliderBind(sol::state& lua);
};

