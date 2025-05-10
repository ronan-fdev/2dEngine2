#pragma once

#include "../../ImGui/imgui_internal.h"

#include "IDisplay.h"

#include "ENGINE/RESOURCES/AssetManager.h"

#include "ENGINE/LOGGER/log.h"

#include "ENGINE/ECS/Registry.h"
#include "ENGINE/ECS/MainRegistry.h"

#include "ENGINE/RENDERER/BUFFERS/FrameBuffer.h"
#include "ENGINE/RENDERER/CORE/Camera2D.h"
#include "ENGINE/RENDERER/CORE/CoreEngineData.h"

#include "ENGINE/SYSTEMS/AnimationSystem.h"
#include "ENGINE/SYSTEMS/PhysicsSystem.h"
#include "ENGINE/SYSTEMS/ScriptingSystem.h"
#include "ENGINE/SYSTEMS/PhysicsSystem.h"
#include "ENGINE/SYSTEMS/SoundSystem.h"

#include "ENGINE/PHYSICS/Box2DWrappers.h"
#include "ENGINE/PHYSICS/ContactListener.h"
#include "ENGINE/PHYSICS/SensorListener.h"

class SceneDisplay : public IDisplay
{
private:
    Registry& m_Registry;
    bool m_bPlayScene, m_bSceneLoaded;

private:
    void LoadScene();
    void UnloadScene();

public:
    SceneDisplay(Registry& registry);
    ~SceneDisplay() = default;

    virtual void Draw() override;
    virtual void Update() override;
};

