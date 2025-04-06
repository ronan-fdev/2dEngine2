#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent(const b2WorldId worldID, const PhysicsAttributes& physicsAttr)
	:
	worldId(worldID),
	m_InitialAttribs(physicsAttr),
	bodyId(b2_nullBodyId),
	shapeId(b2_nullShapeId)
{
}

void PhysicsComponent::Init(int windowWidth, int windowHeight)
{
	if (!b2World_IsValid(worldId))
	{
		LOG_ERROR("Failed to create the Box2D world [{0}]", worldId.index1);
	}

	bool bCircle{ m_InitialAttribs.bCircle };

	//Create the body def
	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = static_cast<b2BodyType>(m_InitialAttribs.eType);
	bodyDef.position = {
		(m_InitialAttribs.position.x + m_InitialAttribs.offset.x - (windowWidth * 0.5f) +
			((bCircle ? m_InitialAttribs.radius : m_InitialAttribs.boxSize.x) *
				m_InitialAttribs.scale.x) * 0.5f) * PIXELS_TO_METERS
		,
			(m_InitialAttribs.position.y + m_InitialAttribs.offset.y - (windowHeight * 0.5f) +
				((bCircle ? m_InitialAttribs.radius : m_InitialAttribs.boxSize.y) *
					m_InitialAttribs.scale.y) * 0.5f) * PIXELS_TO_METERS
	};
	bodyDef.gravityScale = m_InitialAttribs.gravityScale;
	bodyDef.fixedRotation = m_InitialAttribs.bFixedRotation;

	//Create the Ridgid Body:
	bodyId = b2CreateBody(worldId, &bodyDef);
	if (!b2Body_IsValid(bodyId))
	{
		LOG_ERROR("Failed to create the Box2D body [{0}]", bodyId.index1);
	}

	//Create the shape:
	b2Circle circleShape{};
	b2Polygon polyShape{};

	if (m_InitialAttribs.bCircle)
	{
		circleShape.radius = m_InitialAttribs.radius;
		circleShape.center = { 0.0f, 0.0f };
	}
	else if (m_InitialAttribs.bBoxShape)
	{
		polyShape = b2MakeBox(
			PIXELS_TO_METERS * m_InitialAttribs.boxSize.x * m_InitialAttribs.scale.x * 0.5f,
			PIXELS_TO_METERS * m_InitialAttribs.boxSize.y * m_InitialAttribs.scale.y * 0.5f
		);
	}
	else
	{
		// TODO: Create your polygon shape
	}

	//Create the fixture def
	b2ShapeDef fixtureDef = b2DefaultShapeDef();
	fixtureDef.density = m_InitialAttribs.density;
	fixtureDef.friction = m_InitialAttribs.friction;
	fixtureDef.restitution = m_InitialAttribs.restitution;

	if (m_InitialAttribs.bCircle)
	{
		shapeId = b2CreateCircleShape(bodyId, &fixtureDef, &circleShape);
	}
	else
	{
		shapeId = b2CreatePolygonShape(bodyId, &fixtureDef, &polyShape);
	}

	if (!b2Shape_IsValid(shapeId))
	{
		LOG_ERROR("Failed to create the Box2D shape [{0}]", shapeId.index1);
	}
	
}

void PhysicsComponent::CreatePhysicsLuaBind(sol::state& lua, entt::registry& registry)
{
	// TODO: Challenge create the beginning of the lua bindings 
		// Bind the Physics Attributes
		// Bind the component
}
