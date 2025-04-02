#pragma once

#include <iostream>
#include <memory>

#include "ENGINE/WINDOW/Window.h"
#include "ENGINE/LOGGER/log.h"
#include "ENGINE/RESOURCES/AssetManager.h"
#include "ENGINE/RENDERER/CORE/Camera2D.h"
#include "ENGINE/RENDERER/ESSENTIALS/Vertex.h"
#include "ENGINE/ECS/Entity.h";
#include "ENGINE/ECS/COMPONENTS/SpriteComponent.h";
#include "ENGINE/ECS/COMPONENTS/TransformComponent.h";
#include "ENGINE/ECS/COMPONENTS/Identification.h"
		  
#include "ENGINE/SYSTEMS/ScriptingSystem.h"
#include "ENGINE/SYSTEMS/RenderSystem.h"
#include "ENGINE/SYSTEMS/AnimationSystem.h"
#include "ENGINE/SYSTEMS/SoundSystem.h"
		  
#include "ENGINE/SCRIPTING/InputManager.h"
#include "ENGINE/WINDOW/INPUT/Keyboard.h"
#include "ENGINE/WINDOW/INPUT/Mouse.h"

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
};

