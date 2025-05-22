#pragma once

#include "../UTILITIES/EditorUtilities.h"
#include "TileTool.h"

#include "ENGINE/LOGGER/log.h"
#include "ENGINE/RESOURCES/AssetManager.h"
#include "ENGINE/ECS/MainRegistry.h"
#include "ENGINE/RENDERER/CORE/Camera2D.h"

class CreateTileTool : public TileTool
{
public:
	CreateTileTool();
	virtual ~CreateTileTool();

	virtual void Create() override;
	virtual void Draw() override;
};

