#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

struct Color
{
	GLubyte r, g, b, a;
};

struct Vertex
{
	glm::vec2 position{ 0.0f };
	glm::vec2 uv{ 0.0f };
	Color color{ .r = 225,.g = 225,.b = 225,.a = 225 };

	void set_color(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
	{
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
	}
	void set_color(GLuint newColor)
	{
		color.r = (newColor >> 24) & 0xFF;
		color.g = (newColor >> 16) & 0xFF;
		color.b = (newColor >> 8) & 0xFF;
		color.a = (newColor >> 0) & 0xFF;
	}
};
