#pragma once

#include <filesystem>

#include "IDisplay.h"
#include "../../ImGui/imgui_internal.h"
#include "../SCENE/SceneManager.h"
#include "../SCENE/SceneObject.h"

#include "ENGINE/LOGGER/log.h"
#include "ENGINE/FILESYSTEM/DIALOGS/FileDialog.h"
#include "ENGINE/LOADERS/TilemapLoader.h"

class MenuDisplay : public IDisplay
{
private:

public:
	MenuDisplay() = default;
	virtual ~MenuDisplay() = default;

	virtual void Draw() override;
	virtual void Update() override;
};

