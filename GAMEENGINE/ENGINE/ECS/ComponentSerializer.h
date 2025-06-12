#pragma once

#include <rapidjson/document.h>

#include "../FILESYSTEM/SERIALIZERS/JSONSerializer.h"
#include "COMPONENTS/AllComponents.h"

#define SERIALIZE_COMPONENT(serializer, component) ComponentSerializer::Serialize(serializer, component)
#define DESERIALIZE_COMPONENT(COMP, serializer) ComponentSerializer::Deserialize<COMP>(serializer)

class ComponentSerializer
{
public:
	ComponentSerializer() = delete;

	template <typename TComponent, typename TSerializer>
	static void Serialize(TSerializer& serializer, const TComponent& component);

	template <typename TComponent, typename TTable>
	static auto Deserialize(const TTable& table);

private:
	// JSON Serializer

	static void SerializeComponent(JSONSerializer& serializer, const TransformComponent& transform); 
	static void SerializeComponent(JSONSerializer& serializer, const SpriteComponent& sprite); 
	static void SerializeComponent(JSONSerializer& serializer, const AnimationComponent& animation); 
	static void SerializeComponent(JSONSerializer& serializer, const BoxColliderComponent& boxcollider); 
	static void SerializeComponent(JSONSerializer& serializer, const CircleColliderComponent& circlecollider); 
	static void SerializeComponent(JSONSerializer& serializer, const TextComponent& text); 
	static void SerializeComponent(JSONSerializer& serializer, const PhysicsComponent& physics); 
	static void SerializeComponent(JSONSerializer& serializer, const RigidBodyComponent& rigidbody); 

	static TransformComponent DeserializeTransformComponent(const rapidjson::Value& jsonValue);
	static SpriteComponent DeserializeSpriteComponent(const rapidjson::Value& jsonValue);
	static AnimationComponent DeserializeAnimationComponent(const rapidjson::Value& jsonValue);
	static BoxColliderComponent DeserializeBoxColliderComponent(const rapidjson::Value& jsonValue);
	static CircleColliderComponent DeserializeCircleColliderComponent(const rapidjson::Value& jsonValue);
	static TextComponent DeserializeTextComponent(const rapidjson::Value& jsonValue);
	static PhysicsComponent DeserializePhysicsComponent(const rapidjson::Value& jsonValue);
	static RigidBodyComponent DeserializeRigidBodyComponent(const rapidjson::Value& jsonValue);

};

#include "ComponentSerializer.inl"
