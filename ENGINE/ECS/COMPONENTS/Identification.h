#pragma once

#include <string>

struct Identification
{
	std::string name{ "GameObject" }, group{ "" };
	int32_t entity_id{ -1 };
};
