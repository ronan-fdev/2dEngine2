#pragma once

#include <glm/glm.hpp>
#include <sol/sol.hpp>
#include <entt/entt.hpp>

struct RigidBodyComponent
{
    glm::vec2 velocity{ 0.f };

    static void CreateRigidBodyBind(sol::state& lua);
};
