#pragma once

#include <memory>

#include "ENGINE/LOGGER/log.h"
#include "ENGINE/ECS/MainRegistry.h"
#include "ENGINE/RENDERER/CORE/Camera2D.h"
#include "ENGINE/RESOURCES/AssetManager.h"
#include "ENGINE/ECS/Registry.h"
#include "ENGINE/ECS/Entity.h"
#include "ENGINE/RENDERER/CORE/BatchRenderer.h"
#include "ENGINE/RENDERER/CORE/RectBatchRenderer.h"
#include "ENGINE/RENDERER/ESSENTIALS/Primitives.h"


#include "TileTool.h"

constexpr int PRVIEW_LAYER = 10;

class RectFillTool : public TileTool
{
private:
	std::unique_ptr<RectBatchRenderer> m_pShapeRenderer;
	std::unique_ptr<Rect> m_pTileFillRect;
	glm::vec2 m_StartPressPos;

private:
	void CreateTiles();
	void RemoveTiles();
	void DrawPreview(int dx, int dy);
	void ResetTile();

public:
	RectFillTool();
	~RectFillTool();

	virtual void Create() override;
	virtual void Draw() override;
};

