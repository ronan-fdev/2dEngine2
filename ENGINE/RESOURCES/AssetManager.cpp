#include "AssetManager.h"

bool AssetManager::AddTexture(const std::string& textureName, const std::string& texturePath)
{
	if (m_mapTextures.find(textureName) != m_mapTextures.end())
	{
		LOG_WARN("Failed to add texture [{0}] -- Already exists!", textureName);
		return false;
	}
    auto texture = std::move(TextureLoader::createTexture(texturePath.c_str()));
    if (!texture)
    {
        LOG_ERROR("Failed to load texture [{0}] at path [{1}]", textureName, texturePath);
        return false;
    }
    m_mapTextures.emplace(textureName, std::move(texture));
    return true;
}

const Texture& AssetManager::GetTexture(const std::string& textureName)
{
    auto texItr = m_mapTextures.find(textureName);
    if (texItr == m_mapTextures.end())
    {
        LOG_ERROR("Failed to get texture [{0}] -- Does not exist!", textureName);
        return Texture();
    }
    return *texItr->second;
}

bool AssetManager::AddShader(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
{
    // Check to see if the shader already exists
    if (m_mapShader.find(shaderName) != m_mapShader.end())
    {
        LOG_ERROR("Failed to add shader [{0}] -- Already Exists!", shaderName);
        return false;
    }
    // Create and load the shader
    auto shader = std::move(ShaderLoader::createShader(vertexPath.c_str(), fragmentPath.c_str()));
    if (!shader)
    {
        LOG_ERROR("Failed to load Shader [{0}] at vert path [{1}] and frag path [{2}]", shaderName, vertexPath, fragmentPath);
        return false;
    }
    m_mapShader.emplace(shaderName, std::move(shader));
    return true;
}

Shader& AssetManager::GetShader(const std::string& shaderName)
{
    auto shaderItr = m_mapShader.find(shaderName);
    if (shaderItr == m_mapShader.end())
    {
        LOG_ERROR("Failed to get shader [{0}] -- Does not exist!", shaderName);
        Shader shader{};
        return shader;
    }
    return *shaderItr->second;

}

void AssetManager::CreateLuaAssetManager(sol::state& lua, Registry& registry)
{
    auto& asset_manager = registry.GetContext<std::shared_ptr<AssetManager>>();
    if (!asset_manager)
    {
        LOG_ERROR("Failed to bind the asset manager to lua - Does not exist in the registry!");
        return;
    }
    lua.new_usertype<AssetManager>(
        "AssetManager",
        sol::no_constructor,
        "add_texture", [&](const std::string& assetName, const std::string& filepath)
        {
            return asset_manager->AddTexture(assetName, filepath);
        }
    );
}
