#pragma once

#include <box2d/box2d.h>
#include <memory>

#include "../LOGGER/log.h"

struct BodyWrapper
{
	b2BodyId bodyId{ b2_nullBodyId };
	BodyWrapper(const b2BodyId& bId) : bodyId{ bId } {}
};

struct BodyDestroyer
{
	void operator()(BodyWrapper* body) const
	{
		if (b2Body_IsValid(body->bodyId))
		{
			b2DestroyBody(body->bodyId);
			LOG_INFO("Body Destroyed!");
		}
	}
};

static std::shared_ptr<BodyWrapper> MakeSharedBody(b2WorldId worldId, b2BodyDef* bodyDef)
{
	return std::shared_ptr<BodyWrapper>(new BodyWrapper(b2CreateBody(worldId, bodyDef)), BodyDestroyer{});
}

class Box2DWrappers
{
public:
	Box2DWrappers();
	~Box2DWrappers();

	void LoadBox2dWorld();
	void UnLoadBox2dWorld();

	b2WorldId GetWorldID() const { return worldId; }
private:
	b2WorldId worldId;
};

