#pragma once

#include "../ECS/Registry.h"
#include "../RENDERER/CORE/BatchRenderer.h"
#include "../RESOURCES/AssetManager.h"
#include "../ECS/COMPONENTS/TransformComponent.h"
#include "../ECS/COMPONENTS/SpriteComponent.h"
#include "../RENDERER/CORE/Camera2D.h"
#include "../RENDERER/SHADER/Shader.h"
#include "../RENDERER/TEXTURE/Texture.h"
#include "../LOGGER/log.h"

class RenderSystem
{
public:
	RenderSystem(Registry& registry);
	~RenderSystem() = default;
	/*
	* @brief Loops through all of the entities in the registry that have a sprite
	* and transform component. Applies all the necessary transformations and adds them
	* to a Batch to be rendered.
	*/
	void Update();
private:
	Registry& m_Registry;
	std::unique_ptr<SpriteBatchRenderer> m_pBatchRenderer;
};

