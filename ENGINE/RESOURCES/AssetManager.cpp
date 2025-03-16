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

bool AssetManager::AddMusic(const std::string& musicName, const std::string& filepath)
{
    if (m_mapMusic.find(musicName) != m_mapMusic.end())
    {
        LOG_ERROR("Failed to add music [{0}] -- Already exists!", musicName);
        return false;
    }

    // Create the sound parameters
    SoundParams params{
        .name = musicName,
        .filename = filepath,
        .duration = 0.0  // Will be calculated when the music is loaded
    };

    // Create the music object
    auto musicPtr = std::make_shared<Music>(params);
    if (!musicPtr)
    {
        LOG_ERROR("Failed to create the music ptr for [{0}]", musicName);
        return false;
    }

    // Preload the music to get its duration and verify it can be loaded
    if (!musicPtr->Load())
    {
        return false;
    }

    m_mapMusic.emplace(musicName, std::move(musicPtr));
    return true;
}

std::shared_ptr<Music> AssetManager::GetMusic(const std::string& musicName)
{
    auto musicItr = m_mapMusic.find(musicName);
    if (musicItr == m_mapMusic.end())
    {
        LOG_ERROR("Failed to get [{0}] -- Does not exist!", musicName);
        return nullptr;
    }
    return musicItr->second;
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
        },
        "add_music", [&](const std::string& musicName, const std::string& filepath)
        {
            return asset_manager->AddMusic(musicName, filepath);
        }
    );
}
