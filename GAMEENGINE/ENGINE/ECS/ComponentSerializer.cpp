#include "ComponentSerializer.h"

void ComponentSerializer::SerializeComponent(JSONSerializer& serializer, const TransformComponent& transform)
{
	serializer
		.StartNewObject("transform")
			.StartNewObject("position")
				.AddKeyValuePair("x", transform.position.x)
				.AddKeyValuePair("y", transform.position.y)
			.EndNewObject()
			.StartNewObject("scale")
				.AddKeyValuePair("x", transform.scale.x)
				.AddKeyValuePair("y", transform.scale.y)
			.EndNewObject()
			.AddKeyValuePair("rotation", transform.rotation)
		.EndNewObject();
}

void ComponentSerializer::SerializeComponent(JSONSerializer& serializer, const SpriteComponent& sprite)
{
	serializer
		.StartNewObject("sprite")
			.AddKeyValuePair("width", sprite.width)
			.AddKeyValuePair("height", sprite.height)
			.AddKeyValuePair("startX", sprite.start_x)
			.AddKeyValuePair("startY", sprite.start_y)
			.AddKeyValuePair("layer", sprite.layer)
			.AddKeyValuePair("sTexture", sprite.texture_name)
				.StartNewObject("uvs")
					.AddKeyValuePair("u", sprite.uvs.u)
					.AddKeyValuePair("v", sprite.uvs.v)
					.AddKeyValuePair("uv_width", sprite.uvs.uv_width)
					.AddKeyValuePair("uv_height", sprite.uvs.uv_height)
				.EndNewObject()
				.StartNewObject("color")
					.AddKeyValuePair("r", sprite.color.r)
					.AddKeyValuePair("g", sprite.color.g)
					.AddKeyValuePair("b", sprite.color.b)
					.AddKeyValuePair("a", sprite.color.a)
				.EndNewObject()
			.AddKeyValuePair("bHidden", sprite.bHidden)
		.EndNewObject();
}

void ComponentSerializer::SerializeComponent(JSONSerializer& serializer, const AnimationComponent& animation)
{
}

void ComponentSerializer::SerializeComponent(JSONSerializer& serializer, const BoxColliderComponent& boxcollider)
{
}

void ComponentSerializer::SerializeComponent(JSONSerializer& serializer, const CircleColliderComponent& circlecollider)
{
}

void ComponentSerializer::SerializeComponent(JSONSerializer& serializer, const TextComponent& text)
{
}

void ComponentSerializer::SerializeComponent(JSONSerializer& serializer, const PhysicsComponent& physics)
{
}

void ComponentSerializer::SerializeComponent(JSONSerializer& serializer, const RigidBodyComponent& rigidbody)
{
}

TransformComponent ComponentSerializer::DeserializeTransformComponent(const rapidjson::Value& jsonValue)
{
	return TransformComponent{
		.position = glm::vec2{ jsonValue["position"]["x"].GetFloat(), jsonValue["position"]["y"].GetFloat() },
		.scale = glm::vec2{ jsonValue["scale"]["x"].GetFloat(), jsonValue["scale"]["y"].GetFloat() },
		.rotation = jsonValue["rotation"].GetFloat() };
}

SpriteComponent ComponentSerializer::DeserializeSpriteComponent(const rapidjson::Value& jsonValue)
{
	return SpriteComponent{ .width = jsonValue["width"].GetFloat(),
							.height = jsonValue["height"].GetFloat(),
							.uvs = UVs{.u = jsonValue["uvs"]["u"].GetFloat(),
										.v = jsonValue["uvs"]["v"].GetFloat(),
										.uv_width = jsonValue["uvs"]["uv_width"].GetFloat(),
										.uv_height = jsonValue["uvs"]["uv_height"].GetFloat() },
							.start_x = jsonValue["startX"].GetInt(),
							.start_y = jsonValue["startY"].GetInt(),
							.layer = jsonValue["layer"].GetInt(),
							.texture_name = jsonValue["sTexture"].GetString(),
							.bHidden = jsonValue["bHidden"].GetBool() };
}

AnimationComponent ComponentSerializer::DeserializeAnimationComponent(const rapidjson::Value& jsonValue)
{
	return AnimationComponent();
}

BoxColliderComponent ComponentSerializer::DeserializeBoxColliderComponent(const rapidjson::Value& jsonValue)
{
	return BoxColliderComponent();
}

CircleColliderComponent ComponentSerializer::DeserializeCircleColliderComponent(const rapidjson::Value& jsonValue)
{
	return CircleColliderComponent();
}

TextComponent ComponentSerializer::DeserializeTextComponent(const rapidjson::Value& jsonValue)
{
	return TextComponent();
}

PhysicsComponent ComponentSerializer::DeserializePhysicsComponent(const rapidjson::Value& jsonValue)
{
	return PhysicsComponent();
}

RigidBodyComponent ComponentSerializer::DeserializeRigidBodyComponent(const rapidjson::Value& jsonValue)
{
	return RigidBodyComponent();
}
