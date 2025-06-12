#pragma once

#include <string>
#include <filesystem>
#include <rapidjson/error/en.h>

#include "../LOGGER/Log.h"
#include "../ECS/Registry.h"
#include "../ECS/Entity.h"
#include "../ECS/COMPONENTS/AllComponents.h"
#include "../ECS/ComponentSerializer.h"
#include "../FILESYSTEM/SERIALIZERS/JSONSerializer.h"

class TilemapLoader
{
private:
	bool SaveTilemapJSON(Registry& registry, const std::string& sTilemapFile);
	bool LoadTilemapJSON(Registry& registry, const std::string& sTilemapFile);

	// TODO: Save and load functions for lua
public:
	TilemapLoader() = default;
	~TilemapLoader() = default;

	bool SaveTilemap(Registry& registry, const std::string& sTilemapFile, bool bUseJSON = false);
	bool LoadTilemap(Registry& registry, const std::string& sTilemapFile, bool bUseJSON = false);
};