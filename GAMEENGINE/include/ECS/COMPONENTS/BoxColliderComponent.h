#pragma once

struct BoxColliderComponent
{
	int width{ 0 }, height{ 0 };
	glm::vec2 offset{ glm::vec2{ 0 } };
	bool bColliding{ false };
	static void CreateLuaBoxColliderBind( sol::state& lua );
};
