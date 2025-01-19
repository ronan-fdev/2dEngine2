#pragma once

#include <glm/glm.hpp>

struct TransformComponent
{
	glm::vec2 position{ glm::vec2{0.f} }, scale{ glm::vec2{1.} };
	float rotation{ 0.f };
};