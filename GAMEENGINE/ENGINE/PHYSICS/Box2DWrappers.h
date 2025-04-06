#pragma once

#include <box2d/box2d.h>
#include <memory>

#include "../LOGGER/log.h"

class Box2DWrappers
{
public:
	Box2DWrappers();
	~Box2DWrappers();

	b2WorldId GetWorldID() const { return worldId; }
private:
	b2WorldId worldId;
};

