#pragma once

#include "Camera2D.h"
#include "CoreEngineData.h"
#include "../../ECS/Entity.h"
#include "../../ECS/COMPONENTS/TransformComponent.h"

#include <iostream>

struct FollowCamParams
{
    float minX{ 0.f }, minY{ 0.f }, scale{ 1.f }, springback{ 1.f };
    float maxX{ static_cast<float>(CoreEngineData::GetInstance().WindowWidth()) };
    float maxY{ static_cast<float>(CoreEngineData::GetInstance().WindowHeight()) };
};

class FollowCamera
{
private:
    Camera2D& m_Camera;
    FollowCamParams m_Params;
    Entity m_Entity;

public:
    FollowCamera(Camera2D& camera, const Entity& entity, const FollowCamParams& params);
    ~FollowCamera() = default;

    void Update();
    inline void SetCameraParameters(const FollowCamParams& params) { m_Params = params; }
    inline void SetEntity(const Entity& entity) { m_Entity = entity; }
    inline void SetSpringback(float springback) { m_Params.springback = springback; }
    inline const FollowCamParams& GetParams() const { return m_Params; }

    static void CreateLuaFollowCamera(sol::state& lua, Registry& registry);
};

