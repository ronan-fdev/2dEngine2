#pragma once

#include "../../ImGui/imgui.h"

#include "ENGINE/ECS/Registry.h"
#include "ENGINE/RENDERER/BUFFERS/FrameBuffer.h"

class SceneDisplay
{
private:
    Registry& m_Registry;

public:
    SceneDisplay(Registry& registry);
    ~SceneDisplay() = default;

    void Draw();
};

