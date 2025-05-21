#pragma once

#include <string>

#include "../../ImGui/imgui_internal.h"

#include "IDisplay.h"

#include "../SCENE/SceneManager.h"

#include "../SYSTEMS/GridSystem.h"

#include "../UTILITIES/EditorFrameBuffers.h"
#include "../UTILITIES/EditorUtilities.h"

#include "ENGINE/LOGGER/log.h"
#include "ENGINE/ECS/MainRegistry.h"
#include "ENGINE/RESOURCES/AssetManager.h"
#include "ENGINE/RENDERER/CORE/Camera2D.h"
#include "ENGINE/RENDERER/CORE/Renderer.h"

class TilemapDisplay : public IDisplay
{
private:
	std::unique_ptr<Camera2D> m_pTilemapCam;

private:
	void RenderTilemap();

public:
	TilemapDisplay();
	~TilemapDisplay() = default;

	virtual void Draw() override;
	virtual void Update() override;
};

