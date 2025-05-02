#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "../../LOGGER/log.h"


class Texture
{
public:
	enum class TextureType
	{
		PIXEL = 0,
		BLENDED,
		FRAMEBUFFER,
		NONE
	};
	Texture();
	Texture(const char* filePath);
	Texture(const int& width, const int& height);//For the Frame Buffer.
	//~Texture();
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

