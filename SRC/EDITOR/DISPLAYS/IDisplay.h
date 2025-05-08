#pragma once

#include <vector>
#include <memory>

struct IDisplay
{
	virtual ~IDisplay() = default;
	virtual void Draw() = 0;
};

struct DisplayHolder
{
	std::vector<std::unique_ptr<IDisplay>> displays;
};
