#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include <tuple>
#include <string>

#include "../../LOGGER/log.h"

struct TEXTURETYPE
{
	struct FILEPATH {};
	struct FRAMEBUFFER {};
	struct TEXTURE_FROM_MEMORY {};
};

class Texture
{
public:
	Texture();
	//Factory methods to get the texture:
	Texture(TEXTURETYPE::FILEPATH, const char* filePath);
	Texture(TEXTURETYPE::FRAMEBUFFER, const int& width, const int& height);
	Texture(TEXTURETYPE::TEXTURE_FROM_MEMORY, const unsigned char* fileData, int fileSize);

	void InitTextureFromFilePath(const char* filePath);
	void InitTextureFromFrameBuffer(const int& width, const int& height);//For the Frame Buffer.
    void InitTextureFromMemory(const unsigned char* fileData, int fileSize);
	~Texture();
	void bind();
	void unbind();
	unsigned int getID() const { return texture1; }
	int getWidth() const { return width; }
	int getHeight() const { return height; }
private:
	const char* filePath;
	unsigned int texture1;
	int width;
	int height;
};