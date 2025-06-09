#include "MenuDisplay.h"

void MenuDisplay::Draw()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New", "Ctrl + N"))
			{
				LOG_ERROR("NEW PRESSED");
			}

			if (ImGui::MenuItem("Open", "Ctrl + O"))
			{
				std::string basePath = std::filesystem::current_path().string();

				FileDialog fd{};
				auto file = fd.OpenFileDialog("Open tests", basePath, { "*.png", ".jpg" });

				if (!file.empty())
				{
					LOG_INFO("FILE OPENED : {}", file);
				}
			}

			if (ImGui::MenuItem("Save", "Ctrl + S"))
			{
				LOG_ERROR("SAVE PRESSED");
			}

			if (ImGui::MenuItem("Exit"))
			{
				LOG_ERROR("SHOULD EVENTUALLY EXIT!");
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Tools"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Settings"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void MenuDisplay::Update()
{
}
