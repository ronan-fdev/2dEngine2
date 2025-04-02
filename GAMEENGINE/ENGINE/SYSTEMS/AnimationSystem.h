#pragma once

#include "../ECS/Registry.h"
#include "../ECS/COMPONENTS/AnimationComponent.h"
#include "../ECS/COMPONENTS/TransformComponent.h"
#include "../ECS/COMPONENTS/SpriteComponent.h"

class AnimationSystem
{
private:
	Registry& m_Registry;
public:
	AnimationSystem(Registry& registry);
	~AnimationSystem() = default;
	void Update();
};
