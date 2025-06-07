#pragma once

#include <string>
#include <glm/glm.hpp>

#include "../../ImGui/imgui_internal.h"

#include "IDisplay.h"

#include "../SCENE/SceneManager.h"
#include "../SCENE/SceneObject.h"

#include "../TOOLS/ToolManager.h"
#include "../TOOLS/CreateTileTool.h"

#include "../SYSTEMS/GridSystem.h"

#include "../UTILITIES/EditorFrameBuffers.h"
#include "../UTILITIES/EditorUtilities.h"

#include "ENGINE/LOGGER/log.h"
#include "ENGINE/ECS/MainRegistry.h"
#include "ENGINE/RESOURCES/AssetManager.h"
#include "ENGINE/RENDERER/CORE/Camera2D.h"
#include "ENGINE/RENDERER/CORE/Renderer.h"
#include "ENGINE/SYSTEMS/RenderShapeSystem.h"
#include "ENGINE/SYSTEMS/RenderSystem.h"
#include "ENGINE/SYSTEMS/RenderUISystem.h"
#include "ENGINE/SCRIPTING/InputManager.h"
#include "ENGINE/WINDOW/INPUT/Mouse.h"

class TilemapDisplay : public IDisplay
{
private:
	std::unique_ptr<Camera2D> m_pTilemapCam;

private:
	void RenderTilemap();
	void LoadNewScene();
	void PanZoomCamera(const glm::vec2& mousePos);

public:
	TilemapDisplay();
	~TilemapDisplay() = default;

	virtual void Draw() override;
	virtual void Update() override;
};

