#pragma once

#include "../ECS/Registry.h"
#include "../RENDERER/CORE/RectBatchRenderer.h"
#include "../RENDERER/CORE/CircleBatchRenderer.h"
#include "../RENDERER/CORE/Camera2D.h"
#include "../RENDERER/ESSENTIALS/Primitives.h"
#include "../ECS/Components/BoxColliderComponent.h"
#include "../ECS/Components/CircleColliderComponent.h"
#include "../ECS/Components/PhysicsComponent.h"
#include "../ECS/Components/TransformComponent.h"
#include "../Resources/AssetManager.h"
#include "../RENDERER/CORE/CoreEngineData.h"

class RenderShapeSystem
{
private:
    std::unique_ptr<RectBatchRenderer> m_pRectRenderer;
    std::unique_ptr<CircleBatchRenderer> m_pCircleRenderer;

public:
    RenderShapeSystem();
    ~RenderShapeSystem() = default;

    void Update(Registry& registry);
};

