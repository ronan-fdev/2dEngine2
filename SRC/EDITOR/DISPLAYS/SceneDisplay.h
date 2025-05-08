#pragma once

#include "../../ImGui/imgui.h"

#include "IDisplay.h"
#include "ENGINE/ECS/Registry.h"
#include "ENGINE/RENDERER/BUFFERS/FrameBuffer.h"

class SceneDisplay : public IDisplay
{
private:
    Registry& m_Registry;

public:
    SceneDisplay(Registry& registry);
    ~SceneDisplay() = default;

    virtual void Draw() override;
};

