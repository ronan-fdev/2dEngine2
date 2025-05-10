#include "Application.h"

#include "ENGINE/RESOURCES/FONTS/default_font.h"
#include "EDITOR/UTILITIES/EditorTextures.h"

Application& Application::GetInstance()
{
	static Application app{};
	return app;
}

Application::Application()
	:
	pRegistry{ nullptr }, m_bIsRunning{ true }
{
}

Application::~Application()
{
}

void Application::Run()
{
	if (!Initialize())
	{
		LOG_ERROR("Initialization Failed!");
		return;
	}
	while (m_bIsRunning && !Window::shouldClose())
	{
		Window::clearScreen();
		Window::processInput();

		ProcessEvents();
		Update();
		if (Window::getdt() > 0.0f)
		{
			Render();
		}
		Window::update();
		
	}
	CleanUp();
}

bool Application::Initialize()
{
	std::cout << "Hello World!" << std::endl;

	//Logger
	LOGGER_INIT_LOGS(true, true);

	Window::init("2DENGINE");

	auto& mainRegistry = MAIN_REGISTRY();
	mainRegistry.Initialize();

	if (!mainRegistry.GetAssetManager().AddTexture("texture1", "ASSETS/IMAGES/spritesheet.png"))
	{
		LUA_ERROR("Failed to add Default Texture to the Engine from the Asset Manager");
		return false;
	}

	if (!mainRegistry.GetAssetManager().AddFontFromMemory("pixel2", pixel_font))
	{
		LUA_ERROR("Failed to add Default Font to the Engine from the Asset Manager");
	}

	if (!LoadEditorTextures())
	{
		LOG_ERROR("Failed to load the Editor Textures!");
	}

	/*auto assetManager = std::make_shared<AssetManager>();
	if (!assetManager)
	{
		LOG_ERROR("Failed to create the asset manager");
		return -1;
	}

	if (!assetManager->AddTexture("texture1", "ASSETS/IMAGES/spritesheet.png"))
	{
		LOG_ERROR("Failed to add texture!");
		return -1;
	}

	if (!assetManager->AddFontFromMemory("pixel2", pixel_font))
	{
		LOG_ERROR("Failed to add default font to the engine!");
		return -1;
	}*/

	// Create a new entity -- for test
	pRegistry = std::make_unique<Registry>();
	if (!pRegistry)
	{
		LOG_ERROR("Failed to create a entt registry!");
	}

	auto renderer = std::make_shared<Renderer>();
	if (!pRegistry->AddToContext<std::shared_ptr<Renderer>>(renderer))
	{
		LOG_ERROR("Failed to add the renderer to the registry context");
		return false;
	}

	renderer->SetLineWidth(4.f);

	//Create the lua state
	auto lua = std::make_shared<sol::state>();
	if (!lua)
	{
		LOG_ERROR("Failed to create the lua state!");
		return false;
	}

	if (!pRegistry->AddToContext<std::shared_ptr<sol::state>>(lua))
	{
		LOG_ERROR("Failed to add the sol::state to the registry context");
		return false;
	}

	auto scriptSystem = std::make_shared<ScriptingSystem>(*pRegistry);
	if (!scriptSystem)
	{
		LOG_ERROR("Failed to create the script system!");
		return false;
	}

	if (!pRegistry->AddToContext<std::shared_ptr<ScriptingSystem>>(scriptSystem))
	{
		LOG_ERROR("Failed to add the scriptSystem to the registry context");
		return false;
	}

	//Setting up the render system:
	auto renderSystem = std::make_shared<RenderSystem>(*pRegistry);
	if (!renderSystem)
	{
		LOG_ERROR("Failed to create the render system!");
		return false;
	}
	if (!pRegistry->AddToContext<std::shared_ptr<RenderSystem>>(renderSystem))
	{
		LOG_ERROR("Failed to add the rendersystem to the registry context");
		return false;
	}

	//Setting up the render shape system
	auto renderShapeSystem = std::make_shared<RenderShapeSystem>(*pRegistry);
	if (!renderShapeSystem)
	{
		LOG_ERROR("Failed to create the render Shape system!");
		return false;
	}

	if (!pRegistry->AddToContext<std::shared_ptr<RenderShapeSystem>>(renderShapeSystem))
	{
		LOG_ERROR("Failed to add the render Shape system to the registry context!");
		return false;
	}

	//Setting up the animation system
	auto animationSystem = std::make_shared<AnimationSystem>(*pRegistry);
	if (!animationSystem)
	{
		LOG_ERROR("Failed to create the animation system!");
		return false;
	}
	if (!pRegistry->AddToContext<std::shared_ptr<AnimationSystem>>(animationSystem))
	{
		LOG_ERROR("Failed to add the animation system to the registry context!");
		return false;
	}

	// Create a temp camera
	auto camera = std::make_shared<Camera2D>();
	if (!pRegistry->AddToContext<std::shared_ptr<Camera2D>>(camera))
	{
		LOG_ERROR("Failed to add the camera to the registry context!");
		return false;
	}

	if (!LoadShaders())
	{
		LOG_ERROR("Failed to load the shaders!");
		return false;
	}

	//Initialize SoundSystems:
	auto soundSystem = std::make_shared<SoundSystem>();
	if (!pRegistry->AddToContext<std::shared_ptr<SoundSystem>>(soundSystem))
	{
		LOG_ERROR("Failed to add the camera to the registry context!");
		return false;
	}
	if (!soundSystem->Initialize())
	{
		LOG_ERROR("Failed to initialize the SoundSystem");
	}

	if (!soundSystem->LoadBanks("C:/Aswin_Game_DEV/2DEngine2/Project1/ASSETS/SPACESHOOTER/MUSIC/spaceshooter/Build/Desktop"))
	{
		LOG_ERROR("Failed to LoadBanks for SoundSystem");
	}

	//Create the physics 
		//Create the contact binder for the physics world.
	auto pContactListener = std::make_shared<ContactListener>();

	if (!pRegistry->AddToContext<std::shared_ptr<ContactListener>>(pContactListener))
	{
		LOG_ERROR("Failed to add the contact listener to the registry context!");
		return false;
	}
		//Create the sensor binder for the physics world.
	auto pSensorListener = std::make_shared<SensorListener>();

	if (!pRegistry->AddToContext<std::shared_ptr<SensorListener>>(pSensorListener))
	{
		LOG_ERROR("Failed to add the sensor listener to the registry context!");
		return false;
	}
		//Create the physics world
	auto pPhysicsWorld = std::make_shared<Box2DWrappers>();
	//pPhysicsWorld->LoadBox2dWorld();
	if (!pRegistry->AddToContext<std::shared_ptr<Box2DWrappers>>(pPhysicsWorld))
	{
		LOG_ERROR("Failed to add the Physics world to the registry context!");
		return false;
	}
		//Create the physics system
	auto pPhysicsSystem = std::make_shared<PhysicsSystem>(*pRegistry);

	if (!pRegistry->AddToContext<std::shared_ptr<PhysicsSystem>>(pPhysicsSystem))
	{
		LOG_ERROR("Failed to add the Physics system to the registry context!");
		return false;
	}

	//Setting up the RenderUISystem:
	auto renderUISystem = std::make_shared<RenderUISystem>(*pRegistry);
	if (!renderUISystem)
	{
		LOG_ERROR("Failed to create the render UI system!");
		return false;
	}

	if (!pRegistry->AddToContext<std::shared_ptr<RenderUISystem>>(renderUISystem))
	{
		LOG_ERROR("Failed to add the render UI system to the registry context!");
		return false;
	}

	//Setup for the Editor using ImGui
	if (!InitImGui())
	{
		LOG_ERROR("Failed to initialize ImGui!");
		return false;
	}

	if (!CreateDisplays())
	{
		LOG_ERROR("Failed to create displays.");
		return false;
	}
	
	// CREATE TEMP FRAMEBUFFER
	auto pFramebuffer = std::make_shared<FrameBuffer>(640, 480, true);

	if (!pFramebuffer)
	{
		LOG_ERROR("Failed to Create test framebuffer!");
		return false;
	}

	if (!pRegistry->AddToContext<std::shared_ptr<FrameBuffer>>(pFramebuffer))
	{
		LOG_ERROR("Failed add test framebuffer to registry context!");
		return false;
	}

	/*auto pSceneDisplay = std::make_shared<SceneDisplay>(*pRegistry);
	if (!pSceneDisplay)
	{
		LOG_ERROR("Failed to Create test SceneDisplay!");
		return false;
	}

	if (!pRegistry->AddToContext<std::shared_ptr<SceneDisplay>>(pSceneDisplay))
	{
		LOG_ERROR("Failed add test pSceneDisplay to registry context!");
		return false;
	}*/
	
	return true;
}

bool Application::LoadShaders()
{
	auto& mainRegistry = MAIN_REGISTRY();
	auto& assetManager = mainRegistry.GetAssetManager();

	if (!assetManager.AddShader("shader1", "ASSETS/SHADER/shader.vert"
		, "ASSETS/SHADER/shader.frag"))
	{
		LOG_ERROR("Failed to add shader!");
		return false;
	}

	if (!assetManager.AddShader("color", "ASSETS/SHADER/colorShader.vert",
		"ASSETS/SHADER/colorShader.frag"))
	{
		LOG_ERROR("Failed to add the color shader to the asset manager");
		return false;
	}

	if (!assetManager.AddShader("circle", "ASSETS/SHADER/circleShader.vert",
		"ASSETS/SHADER/circleShader.frag"))
	{
		LOG_ERROR("Failed to add the color shader to the asset manager");
		return false;
	}

	if (!assetManager.AddShader("font", "ASSETS/SHADER/fontShader.vert",
		"ASSETS/SHADER/fontShader.frag"))
	{
		LOG_ERROR("Failed to add the font shader to the asset manager");
		return false;
	}

	return true;
}

bool Application::LoadEditorTextures()
{
	auto& mainRegistry = MAIN_REGISTRY();
	auto& assetManager = mainRegistry.GetAssetManager();

	if (!assetManager.AddTextureFromMemory(
		"play_button", play_button, sizeof(play_button) / sizeof(play_button[0])))
	{
		LOG_ERROR("Failed to load texture [play_button] from memory.");
		return false;
	}

	if (!assetManager.AddTextureFromMemory(
		"stop_button", stop_button, sizeof(stop_button) / sizeof(stop_button[0])))
	{
		LOG_ERROR("Failed to load texture [stop_button] from memory.");
		return false;
	}

	return true;
}

void Application::ProcessEvents()
{
	auto& inputManager = InputManager::GetInstance();
	auto& keyboard = inputManager.GetKeyboard();
	auto& mouse = inputManager.GetMouse();

	// Process Events
	glfwPollEvents();

	// Check if window should close
	if (glfwWindowShouldClose(Window::getGLFWWindow())) {
		m_bIsRunning = false;
	}

	// Check ESC key directly for quitting
	if (glfwGetKey(Window::getGLFWWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		m_bIsRunning = false;
	}

	// Instead of directly handling keyboard events here, you'll want to set up GLFW callbacks
	// Add this during window/input initialization:
	glfwSetKeyCallback(Window::getGLFWWindow(), [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

		auto& inputManager = InputManager::GetInstance();
		auto& keyboard = inputManager.GetKeyboard();

		if (action == GLFW_PRESS) {
			keyboard.OnKeyPressed(key);
		}
		else if (action == GLFW_RELEASE) {
			keyboard.OnKeyReleased(key);
		}
	});

	// Set up mouse button callback
	glfwSetMouseButtonCallback(Window::getGLFWWindow(), [](GLFWwindow* window, int button, int action, int mods) {
		// 1. Forward to ImGui first
		ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

		auto& inputManager = InputManager::GetInstance();
		auto& mouse = inputManager.GetMouse();

		if (action == GLFW_PRESS) {
			mouse.OnBtnPressed(button);
		}
		else if (action == GLFW_RELEASE) {
			mouse.OnBtnReleased(button);
		}
	});

	// Set up mouse scroll callback
	glfwSetScrollCallback(Window::getGLFWWindow(), [](GLFWwindow* window, double xoffset, double yoffset) {
		ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);

		auto& inputManager = InputManager::GetInstance();
		auto& mouse = inputManager.GetMouse();
		mouse.SetMouseWheelX(static_cast<int>(xoffset));
		mouse.SetMouseWheelY(static_cast<int>(yoffset));
	});

	// Set up mouse movement callback
	glfwSetCursorPosCallback(Window::getGLFWWindow(), [](GLFWwindow* window, double xpos, double ypos) {
		ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);

		auto& inputManager = InputManager::GetInstance();
		auto& mouse = inputManager.GetMouse();
		mouse.SetMouseMoving(true);
	});
}

void Application::Update()
{
	auto& mainRegistry = MAIN_REGISTRY();
	auto& displayHolder = mainRegistry.GetContext<std::shared_ptr<DisplayHolder>>();
	for (const auto& pDisplay : displayHolder->displays)
	{
		pDisplay->Update();
	}

	auto view = pRegistry->GetRegistry().view<TransformComponent, SpriteComponent>();

	// Update inputs
	auto& inputManager = InputManager::GetInstance();
	auto& keyboard = inputManager.GetKeyboard();
	keyboard.Update();
	auto& mouse = inputManager.GetMouse();
	mouse.Update();
	//Update SoundSystem
	auto& soundSystem = pRegistry->GetContext<std::shared_ptr<SoundSystem>>();
	soundSystem->Update(Window::getdt(), *pRegistry);

}

void Application::Render()
{
	auto& renderSystem = pRegistry->GetContext<std::shared_ptr<RenderSystem>>();
	auto& renderShapeSystem = pRegistry->GetContext<std::shared_ptr<RenderShapeSystem>>();
	auto& renderUISystem = pRegistry->GetContext<std::shared_ptr<RenderUISystem>>();
	
	auto& camera = pRegistry->GetContext<std::shared_ptr<Camera2D>>();
	auto& renderer = pRegistry->GetContext<std::shared_ptr<Renderer>>();
	auto& mainRegistry = MAIN_REGISTRY();
	auto& assetManager = mainRegistry.GetAssetManager();

	auto& shader = assetManager.GetShader("color");
	auto& circleShader = assetManager.GetShader("circle");
	auto& fontShader = assetManager.GetShader("font");

	//auto& scriptSystem = pRegistry->GetContext<std::shared_ptr<ScriptingSystem>>();

	const auto& fb = pRegistry->GetContext<std::shared_ptr<FrameBuffer>>();

	fb->Bind();

	glViewport(0, 0, fb->Width(), fb->Height());
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//scriptSystem->Render();
	renderSystem->Update();
	renderShapeSystem->Update();
	renderUISystem->Update(pRegistry->GetRegistry());

	fb->Unbind();

	Begin();
	RenderImGui();
	End();

	renderer->DrawLines(shader, *camera);
	renderer->DrawFilledRects(shader, *camera);
	renderer->DrawCircles(circleShader, *camera);
	renderer->DrawAllText(fontShader, *camera);

	renderer->ClearPrimitives();

	fb->CheckResize();
}

void Application::CleanUp()
{
	Window::cleanup();
}

bool Application::CreateDisplays()
{
	auto& mainRegistry = MAIN_REGISTRY();

	auto pDisplayHolder = std::make_shared<DisplayHolder>();

	if (!mainRegistry.AddToContext<std::shared_ptr<DisplayHolder>>(pDisplayHolder))
	{
		LOG_ERROR("Failed to add the display holder to the main registry.");
		return false;
	}

	auto pSceneDisplay = std::make_unique<SceneDisplay>(*pRegistry);
	if (!pSceneDisplay)
	{
		LOG_ERROR("Failed to Create Scene Display!");
		return false;
	}

	auto pLogDisplay = std::make_unique<LogDisplay>();
	if (!pLogDisplay)
	{
		LOG_ERROR("Failed to Create Log Display!");
		return false;
	}

	// TODO: Create and add other displays as needed

	pDisplayHolder->displays.push_back(std::move(pSceneDisplay));
	pDisplayHolder->displays.push_back(std::move(pLogDisplay));

	return true;
}

bool Application::InitImGui()
{
	const char* glslVersion = "#version 330";//This sets the GLSL (OpenGL Shading Language) version to 330
	
	IMGUI_CHECKVERSION();/*This macro checks that the version of the compiled ImGui code matches the version used in the application.
		It's a safeguard to prevent mismatches that could cause undefined behavior.*/

	if (!ImGui::CreateContext())//Creates the core ImGui context, which is necessary before any other ImGui function can be used.
	{
		LOG_ERROR("Failed to create ImGui Context");
		return false;
	}

	ImGuiIO& io = ImGui::GetIO();//Retrieves the ImGuiIO object, which handles configuration and input/output for ImGui.
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;//Enables docking, which allows ImGui windows to be docked into each other or a main window.
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;//Enables keyboard navigation through UI elements.
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;//Enables multi-viewport support, allowing ImGui windows to be dragged outside the main application window

	io.ConfigWindowsMoveFromTitleBarOnly = true;/*When set to true, ImGui windows can only be moved by dragging their title bars instead of any region.
		This prevents accidental dragging when interacting with the window contents.*/

	if (!ImGui_ImplGlfw_InitForOpenGL(
		Window::getGLFWWindow(), false
	))/*Initializes the GLFW backend of ImGui for use with OpenGL.

		Window::getGLFWWindow() retrieves the pointer to your GLFW window.

		The second parameter(install_callbacks) is set to false, meaning you’ll manually handle GLFW callbacks like key presses, mouse movement, etc., 
		instead of letting ImGui install its own.*/
	{
		LOG_ERROR("Failed to intialize ImGui GLFW for OpenGL!");
		return false;
	}

	if (!ImGui_ImplOpenGL3_Init(glslVersion))/*Initializes the OpenGL 3 backend of ImGui using the previously defined GLSL version("#version 330").
		This sets up shaders, buffers, etc., required to render ImGui with OpenGL.*/
	{
		LOG_ERROR("Failed to intialize ImGui OpenGL3!");
		return false;
	}

	return true;
}

void Application::Begin()
{
	ImGui_ImplOpenGL3_NewFrame();//Prepares the OpenGL3 backend for a new frame. Updates GPU-related state like shaders and buffers.
	ImGui_ImplGlfw_NewFrame();//Prepares the GLFW backend for the new frame. Processes input (mouse, keyboard, etc.) from GLFW.
	ImGui::NewFrame();//Starts a new ImGui frame internally. After this call, you can begin defining your ImGui UI.
}

void Application::End()
{
	ImGui::Render();//This function finalizes the current ImGui frame.
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());//Renders the ImGui draw data using the OpenGL3 backend.

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		// Backup current context
		GLFWwindow* backupContext = glfwGetCurrentContext();//Saves the current OpenGL context (your main window’s rendering context).

		ImGui::UpdatePlatformWindows();//Updates all platform windows created by ImGui.
		ImGui::RenderPlatformWindowsDefault();//Renders ImGui draw data to each platform window.

		// Restore original context
		glfwMakeContextCurrent(backupContext);//Restores the original OpenGL context to avoid breaking rendering in the main window.
	}
}


void Application::RenderImGui()
{
	const auto dockSpaceId = ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());//Creates a Docking Space that fills the main viewport (usually your full application window).

	if (static auto firstTime = true; firstTime) [[unlikely]]//This runs only once, the first time RenderImGui() is called. [[unlikely]] hints to the compiler that this is rarely executed 
		{
			firstTime = false;

			ImGui::DockBuilderRemoveNode(dockSpaceId);//Remove and re-add the docking node to ensure a fresh layout state.
			ImGui::DockBuilderAddNode(dockSpaceId);//Remove and re-add the docking node to ensure a fresh layout state.

			auto centerNodeId = dockSpaceId;
			const auto leftNodeId =
				ImGui::DockBuilderSplitNode(centerNodeId, ImGuiDir_Left, 0.2f, nullptr, &centerNodeId);
			/*Split the dock node into a left pane(20 % width) and remaining center pane.
				The original dockSpaceId becomes the left node; centerNodeId becomes the center portion.*/

			const auto LogNodeId = ImGui::DockBuilderSplitNode(centerNodeId, ImGuiDir_Down, 0.25f, nullptr, &centerNodeId);
			//Further splits the center node downward to create a bottom log panel (25% height).
			ImGui::DockBuilderDockWindow("Dear ImGui Demo", leftNodeId);//Assigns named ImGui windows to specific dock areas
			ImGui::DockBuilderDockWindow("Scene", centerNodeId);//Assigns named ImGui windows to specific dock areas 
			ImGui::DockBuilderDockWindow("Logs", LogNodeId);//Assigns named ImGui windows to specific dock areas 

			ImGui::DockBuilderFinish(dockSpaceId);//Finalizes the layout and applies it.
		}


	// TODO: Add new Scene Display!
	auto& mainRegistry = MAIN_REGISTRY();
	auto& pDisplayHolder = mainRegistry.GetContext<std::shared_ptr<DisplayHolder>>();

	for (const auto& pDisplay : pDisplayHolder->displays)
	{
		pDisplay->Draw();
	}


	ImGui::ShowDemoWindow();//Displays the built-in ImGui demo window, useful for testing and exploration.
}
