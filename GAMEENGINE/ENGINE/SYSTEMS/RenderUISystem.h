#pragma once

#include "../RENDERER/CORE/BatchRenderer.h"
#include "../RENDERER/CORE/TextBatchRenderer.h"
#include "../RENDERER/CORE/Camera2D.h"
#include "../RENDERER/ESSENTIALS/Font.h"
#include "../ECS/Registry.h"
#include "../ECS/COMPONENTS/TextComponent.h"
#include "../ECS/COMPONENTS/TransformComponent.h"
#include "../RESOURCES/AssetManager.h"
#include "../LOGGER/log.h"

class RenderUISystem
{
private:
    std::unique_ptr<SpriteBatchRenderer> m_pSpriteRenderer;
    std::unique_ptr<TextBatchRenderer> m_pTextRenderer;
    std::unique_ptr<Camera2D> m_pCamera2D;

public:
    RenderUISystem();
    ~RenderUISystem() = default;

    void Update(Registry& registry, Camera2D& camera);
};

