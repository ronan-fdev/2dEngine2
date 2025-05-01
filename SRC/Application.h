#pragma once

#include <iostream>
#include <memory>

#include "ENGINE/WINDOW/Window.h"
#include "ENGINE/LOGGER/log.h"
#include "ENGINE/RESOURCES/AssetManager.h"
#include "ENGINE/RENDERER/CORE/Camera2D.h"
#include "ENGINE/RENDERER/CORE/Renderer.h"
#include "ENGINE/RENDERER/ESSENTIALS/Vertex.h"

#include "ENGINE/ECS/Entity.h";
#include "ENGINE/ECS/COMPONENTS/SpriteComponent.h";
#include "ENGINE/ECS/COMPONENTS/TransformComponent.h";
#include "ENGINE/ECS/COMPONENTS/Identification.h"
#include "ENGINE/ECS/COMPONENTS/PhysicsComponent.h"
#include "ENGINE/ECS/COMPONENTS/BoxColliderComponent.h"
#include "ENGINE/ECS/COMPONENTS/CircleColliderComponent.h"
#include "ENGINE/ECS/COMPONENTS/TextComponent.h"

#include "ENGINE/PHYSICS/ContactListener.h"
		  
#include "ENGINE/SYSTEMS/ScriptingSystem.h"
#include "ENGINE/SYSTEMS/RenderSystem.h"
#include "ENGINE/SYSTEMS/AnimationSystem.h"
#include "ENGINE/SYSTEMS/SoundSystem.h"
#include "ENGINE/SYSTEMS/PhysicsSystem.h"
#include "ENGINE/SYSTEMS/RenderShapeSystem.h"
#include "ENGINE/SYSTEMS/RenderUISystem.h"
		  
#include "ENGINE/SCRIPTING/InputManager.h"
#include "ENGINE/WINDOW/INPUT/Keyboard.h"
#include "ENGINE/WINDOW/INPUT/Mouse.h"

#include "Imgui/imgui.h"
#include "Imgui/backends/imgui_impl_glfw.h"
#include "Imgui/backends/imgui_impl_opengl3.h"

class Application
{
public:
	Application();
	~Application();
	std::unique_ptr<Registry> pRegistry;
	bool m_bIsRunning;

	static Application& GetInstance();
	void Run();
private:
	bool Initialize();
	bool LoadShaders();
	void ProcessEvents();
	void Update();
	void Render();
	void CleanUp();

	//TEMP IMGUI TESTING --> TODO: REMOVE TO OWN CLASS
	bool InitImGui();
	void Begin();
	void End();
	void RenderImGui();
};

