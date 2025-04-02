#pragma once

#include <memory>

#include "Texture.h"

class TextureLoader
{
public:
	static std::shared_ptr<Texture> createTexture(const char* filePath)
	{
		return std::make_shared<Texture>(filePath);
	}
};
