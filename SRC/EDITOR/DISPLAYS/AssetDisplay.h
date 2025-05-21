#pragma once

#include <string>

#include "ENGINE/LOGGER/log.h"
#include "ENGINE/UTILITIES/2dEngineUtilities.h"
#include "ENGINE/ECS/MainRegistry.h"
#include "ENGINE/SCRIPTING/InputManager.h"
#include "ENGINE/RESOURCES/AssetManager.h"

#include "../../IMGUI/imgui_internal.h"

#include "IDisplay.h"

#include "../UTILITIES/EditorUtilities.h"

#include "../SCENE/SceneManager.h"

class AssetDisplay : public IDisplay
{
private:
	const std::vector<std::string> m_SelectableTypes{ "TEXTURES", "FONTS", "MUSIC", "SOUNDFX", "SCENES" };
	bool m_bAssetTypeChanged, m_bRename;
	bool m_bWindowSelected, m_bWindowHovered;
	std::string m_sSelectedAssetName, m_sSelectedType, m_sDragSource, m_sRenameBuf;
	AssetType m_eSelectedType;
	float m_AssetSize;
	int m_SelectedID;

private:
	void SetAssetType();
	void DrawSelectedAssets();
	unsigned int GetTextureID(const std::string& sAssetName) const;
	bool DoRenameAsset(const std::string& sOldName, const std::string& sNewName) const;
	void CheckRename(const std::string& sCheckName) const;
	void OpenAssetContext(const std::string& sAssetName);

public:
	AssetDisplay();
	~AssetDisplay() = default;

	virtual void Draw() override;
	virtual void Update() override;
};

