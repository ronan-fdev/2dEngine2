#pragma once

#include <string>

#include "../../ImGui/imgui_internal.h"

#include "IDisplay.h"
#include "ENGINE//LOGGER/log.h"
#include "ENGINE/RESOURCES/AssetManager.h"
#include "ENGINE/ECS/MainRegistry.h"

class TilesetDisplay : public IDisplay
{
private:
	std::string m_sTileset{ "" };
	int m_Selected{ -1 };

public:
	TilesetDisplay() = default;
	~TilesetDisplay() = default;

	virtual void Draw() override;

	inline void SetTileset(const std::string& sTileset) { m_sTileset = sTileset; }
	inline const std::string& GetTilesetName() const { return m_sTileset; }
};

