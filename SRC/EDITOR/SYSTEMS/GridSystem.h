#pragma once

#include <memory>

#include "ENGINE/ECS/MainRegistry.h"
#include "ENGINE/RESOURCES/AssetManager.h"
#include "ENGINE/RENDERER/CORE/RectBatchRenderer.h"
#include "ENGINE/RENDERER/CORE/Camera2D.h"
#include "ENGINE/RENDERER/ESSENTIALS/Primitives.h"
#include "ENGINE/RENDERER/ESSENTIALS/Vertex.h"

#include "../SCENE/SceneObject.h"

class RectBatchRenderer;
class Camera2D;

class GridSystem
{
private:
	std::unique_ptr<RectBatchRenderer> m_pBatchRenderer;

public:
	GridSystem();
	~GridSystem() = default;

	void Update(SceneObject& currentScene, Camera2D& camera);
};

