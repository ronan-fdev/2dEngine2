#pragma once

#include <sol/sol.hpp>
#include <glm/glm.hpp>
#include <entt/entt.hpp>

#include "../../LOGGER/log.h"
#include "../../ECS/Registry.h"
#include "../../PHYSICS/Box2DWrappers.h"
#include "../../RENDERER/CORE/CoreEngineData.h"

enum class RigidBodyType { STATIC = 0, KINEMATIC, DYNAMIC };

struct PhysicsAttributes
{
    RigidBodyType eType{ RigidBodyType::STATIC };
    float density{ 1.f }, friction{ 0.2f }, restitution{ 0.2f };
    float restitutionThreshold{ 1.f }, radius{ 0.f }, gravityScale{ 1.f };

    glm::vec2 position{ 0.f }, scale{ 1.f }, boxSize{ 0.f }, offset{ 0.f };
    bool bCircle{ false }, bBoxShape{ true }, bFixedRotation{ true };

    uint16_t filterCategory{ 0 }, filterMask{ 0 };
    int16_t groupIndex{ 0 };
};

class PhysicsComponent
{
public:
    PhysicsComponent() = default;
    PhysicsComponent(const b2WorldId worldId, const PhysicsAttributes& physicsAttr);
    ~PhysicsComponent() = default;

    void Init(int windowWidth, int windowHeight);
    b2BodyId getBodyID() const { return bodyId; }
    glm::vec2 BodyPosition();

    static void CreatePhysicsLuaBind(sol::state& lua, Registry& registry);

private:
    b2WorldId worldId;
    b2BodyId bodyId;
    b2ShapeId shapeId;

    PhysicsAttributes m_InitialAttribs;

};

