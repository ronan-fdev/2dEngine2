#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "../../LOGGER/log.h"


class Texture
{
public:
	Texture();
	Texture(const char* filePath);
	void bind();
	void unbind();
	unsigned int getID() { return texture1; }
	int getWidth() const { return width; }
	int getHeight() const { return height; }
private:
	const char* filePath;
	unsigned int texture1;
	int width;
	int height;
};

