#include "SceneDisplay.h"

SceneDisplay::SceneDisplay(Registry& registry)
	: m_Registry{ registry }
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
        auto& fb = m_Registry.GetContext<std::shared_ptr<FrameBuffer>>();

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