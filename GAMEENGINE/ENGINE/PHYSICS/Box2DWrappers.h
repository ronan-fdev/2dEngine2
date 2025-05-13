#pragma once

#include <box2d/box2d.h>
#include <memory>

#include "../LOGGER/log.h"
#include "SensorListener.h"

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
			auto& sensorMapRef = SensorListener::sensorData;

			for (auto it = sensorMapRef.begin(); it != sensorMapRef.end(); it++)
			{
				const auto& [bodyA, bodyB] = it->first;

				if (bodyA.index1 == body->bodyId.index1 || bodyB.index1 == body->bodyId.index1)
				{
					LOG_INFO("Removing sensor contact data for body {}", body->bodyId.index1);
					sensorMapRef.erase(it); // Remove and advance iterator safely
				}

			}

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

