#include "SceneDisplay.h"

void SceneDisplay::LoadScene()
{
    auto& physicsWrappers = m_Registry.GetContext<std::shared_ptr<Box2DWrappers>>();
    auto& scriptSystem = m_Registry.GetContext<std::shared_ptr<ScriptingSystem>>();
    auto& lua = m_Registry.GetContext<std::shared_ptr<sol::state>>();

    physicsWrappers->LoadBox2dWorld();

    if (!lua)
        lua = std::make_shared<sol::state>();

    lua->open_libraries(sol::lib::base,
        sol::lib::math,
        sol::lib::os,
        sol::lib::table,
        sol::lib::io,
        sol::lib::string,
        sol::lib::package);

    ENGINE_CRASH_LOGGER().SetLuaState(*lua);

    ScriptingSystem::RegisterLuaBindings(*lua, m_Registry);
    ScriptingSystem::RegisterLuaFunctions(*lua, m_Registry);

    if (!scriptSystem->LoadMainScript(*lua))
    {
        LOG_ERROR("Failed to load the main lua script!");
        return;
    }

    m_bSceneLoaded = true;
    m_bPlayScene = true;
}

void SceneDisplay::UnloadScene()
{
    auto& physicsWrappers = m_Registry.GetContext<std::shared_ptr<Box2DWrappers>>();
    m_bPlayScene = false;
    m_bSceneLoaded = false;
    m_Registry.GetRegistry().clear();
    physicsWrappers->UnLoadBox2dWorld();

    auto& sensorListener = m_Registry.GetContext<std::shared_ptr<SensorListener>>();
    sensorListener->ResetUserSensorAB();

    auto& contactListener = m_Registry.GetContext<std::shared_ptr<ContactListener>>();
    contactListener->ResetUserContactsAB();

    auto& lua = m_Registry.GetContext<std::shared_ptr<sol::state>>();
    lua.reset();
}

void SceneDisplay::RenderScene()
{
    auto& mainRegistry = MAIN_REGISTRY();
    auto& editorFramebuffers = mainRegistry.GetContext<std::shared_ptr<EditorFramebuffers>>();
    auto& renderer = mainRegistry.GetContext<std::shared_ptr<Renderer>>();

    auto& renderSystem = mainRegistry.GetContext<std::shared_ptr<RenderSystem>>();
    auto& renderUISystem = mainRegistry.GetContext<std::shared_ptr<RenderUISystem>>();
    auto& renderShapeSystem = mainRegistry.GetContext<std::shared_ptr<RenderShapeSystem>>();

    const auto& fb = editorFramebuffers->mapFramebuffers[FramebufferType::SCENE];

    fb->Bind();

    glViewport(0, 0, fb->Width(), fb->Height());
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //scriptSystem->Render();
    renderSystem->Update();
    renderShapeSystem->Update();
    renderUISystem->Update(m_Registry.GetRegistry());

    fb->Unbind();

    fb->CheckResize();

}

SceneDisplay::SceneDisplay(Registry& registry)
	: m_Registry{ registry }
    , m_bPlayScene{ false }
    , m_bSceneLoaded{ false }
{
}

//void SceneDisplay::Draw()
//{
//    static bool pOpen{ true };
//    if (!ImGui::Begin("Scene", &pOpen))
//    {
//        ImGui::End();
//        return;
//    }
//
//    if (ImGui::BeginChild("##SceneChild", ImVec2{ 0.f, 0.f }, NULL, ImGuiWindowFlags_NoScrollWithMouse))
//    {
//        const auto& fb = m_Registry.GetContext<std::shared_ptr<FrameBuffer>>();
//
//        ImGui::SetCursorPos(ImVec2{ 0.f, 0.f });
//
//        ImGui::Image(
//            (ImTextureID)fb->GetTextureID(),
//            ImVec2{
//                static_cast<float>(fb->Width()),
//                static_cast<float>(fb->Height())
//            },
//            ImVec2{ 0.f, 1.f }, ImVec2{ 1.f, 0.f }
//        );
//        ImGui::EndChild();
//
//        // Check for resize based on the window size
//        ImVec2 windowSize{ ImGui::GetWindowSize() };
//        if (fb->Width() != static_cast<int>(windowSize.x) || fb->Height() != static_cast<int>(windowSize.y))
//            fb->Resize(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));
//    }
//
//    ImGui::End();
//}

void SceneDisplay::Draw()
{
    static bool pOpen{ true };
    if (!ImGui::Begin("Scene", &pOpen)) {
        ImGui::End();
        return;
    }
    /*Begins an ImGui window titled "Scene".
        If the window is collapsed or not visible, it immediately ends the window and returns early.*/

    auto& mainRegistry = MAIN_REGISTRY();
    auto& assetManager = mainRegistry.GetAssetManager();

    auto& pPlayTexture = assetManager.GetTexture("play_button");
    auto& pStopTexture = assetManager.GetTexture("stop_button");

    const bool isScenePlaying = m_bPlayScene;
    const bool isSceneLoaded = m_bSceneLoaded;

    if (isScenePlaying)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.f, 0.9f, 0.f, 0.3 });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.f, 0.9f, 0.f, 0.3 });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.f, 0.9f, 0.f, 0.3 });
    }

    if (ImGui::ImageButton("##play", (ImTextureID)pPlayTexture.getID(),
        ImVec2{
            (float)pPlayTexture.getWidth() * 0.25f,
            (float)pPlayTexture.getHeight() * 0.25f,
        }) &&
        !isSceneLoaded)
    {
        LoadScene();
    }

    if (isScenePlaying)
        ImGui::PopStyleColor(3);

    /*if (ImGui::GetColorStackSize() > 0)
        ImGui::PopStyleColor(ImGui::GetColorStackSize());*/

    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
        ImGui::SetTooltip("Play Scene");

    ImGui::SameLine();

    const bool isSceneNotPlaying = !isScenePlaying;

    if (isSceneNotPlaying)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.f, 0.9f, 0.f, 0.3 });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.f, 0.9f, 0.f, 0.3 });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.f, 0.9f, 0.f, 0.3 });
    }

    RenderScene();

    if (ImGui::ImageButton("##stop",(ImTextureID)pStopTexture.getID(),
        ImVec2{
            (float)pStopTexture.getWidth() * 0.25f,
            (float)pStopTexture.getHeight() * 0.25f,
        }) &&
        isSceneLoaded)
    {
        UnloadScene();
    }

    if (isSceneNotPlaying)
        ImGui::PopStyleColor(3);

    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
        ImGui::SetTooltip("Stop Scene");

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    /*Removes the default padding inside the window.
        This is important because we want the framebuffer image to take up the entire window content without extra spacing.*/

    if (ImGui::BeginChild("##SceneChild", ImVec2(0, 0), false,
        ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar))
        /*Creates a child region inside the Scene window with :
            No scrolling
            No scrollbars
            Full size(ImVec2(0, 0) means it expands to fill available space)
        "##SceneChild": The ## prefix hides the label but still gives it a unique internal ID.*/
    {
        auto& editorFramebuffers = mainRegistry.GetContext<std::shared_ptr<EditorFramebuffers>>();
        const auto& fb = editorFramebuffers->mapFramebuffers[FramebufferType::SCENE];

        // 1. FIRST get available space
        ImVec2 availableSize = ImGui::GetContentRegionAvail();//Queries the available drawable area inside the child window.

        // 2. THEN resize if needed (before rendering)
        if (fb->Width() != (int)availableSize.x || fb->Height() != (int)availableSize.y)
            fb->Resize((int)availableSize.x, (int)availableSize.y);

        // 3. NOW display at correct size
        ImGui::SetCursorPos(ImVec2(0, 0));
        ImGui::Image(
            (ImTextureID)fb->GetTextureID(),
            availableSize, // Use validated available size
            ImVec2(0, 1), ImVec2(1, 0)//These UV coordinates flip the image vertically
        );

        ImGui::EndChild();
    }
    ImGui::PopStyleVar();
    ImGui::End();
}

void SceneDisplay::Update()
{
    if (!m_bPlayScene)
        return;

    auto& mainRegistry = MAIN_REGISTRY();
    auto& coreGlobals = CORE_GLOBALS();

    auto& scriptSystem = m_Registry.GetContext<std::shared_ptr<ScriptingSystem>>();
    scriptSystem->Update();

    auto& camera = m_Registry.GetContext<std::shared_ptr<Camera2D>>();
    if (!camera)
    {
        LOG_ERROR("Failed to get the camera from the registry context!");
        return;
    }
    camera->Update();

    auto& animationSystem = m_Registry.GetContext<std::shared_ptr<AnimationSystem>>();
    animationSystem->Update();

    ////Update SoundSystem
    //auto& soundSystem = m_Registry.GetContext<std::shared_ptr<SoundSystem>>();
    //soundSystem->Update(Window::getdt(), m_Registry);

    auto& physics = m_Registry.GetContext<std::shared_ptr<Box2DWrappers>>();

    if (coreGlobals.IsPhysicsEnabled())
    {
        //Update physics and physics system
        float timeStep = 1.0f / 60.0f;
        int subStepCount = 4;
        // Perform physics step
        b2World_Step(physics->GetWorldID(), timeStep, subStepCount);
    }

    // Reset active contacts before processing physics
    auto& pSensorListener = m_Registry.GetContext<std::shared_ptr<SensorListener>>();
    // Process sensor contacts
    pSensorListener->ProcessSensorContactEvents(physics->GetWorldID());

    // Process contacts
    auto& pContactListener = m_Registry.GetContext<std::shared_ptr<ContactListener>>();
    pContactListener->BeginContact(physics->GetWorldID());
    pContactListener->EndContact(physics->GetWorldID());

    // Update physics system
    auto& pPhysicsSystem = m_Registry.GetContext<std::shared_ptr<PhysicsSystem>>();
    pPhysicsSystem->Update(m_Registry.GetRegistry());
}
