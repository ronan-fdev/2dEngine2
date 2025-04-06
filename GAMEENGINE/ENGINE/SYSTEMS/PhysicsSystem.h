#pragma once

#include "../LOGGER/log.h"

#include "../ECS/Registry.h"
#include "../ECS/Components/BoxColliderComponent.h"
#include "../ECS/Components/CircleColliderComponent.h"
#include "../ECS/Components/TransformComponent.h"
#include "../ECS/Components/PhysicsComponent.h"

class PhysicsSystem
{
private:
    Registry& m_Registry;

public:
    PhysicsSystem(Registry& registry);
    ~PhysicsSystem() = default;

    void Update(entt::registry& registry);
};

