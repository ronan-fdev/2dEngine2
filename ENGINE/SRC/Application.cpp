#include "Application.h"

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
	log::Init();

	Window::init(640, 480, "2DENGINE");

	auto assetManager = std::make_shared<AssetManager>();
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
	auto texture = assetManager->GetTexture("texture1");

	// Let's make some temporary UVs
	UVs uVs{};
	LOG_INFO("Loaded Texture: [width = {0}, height = {1}]", texture.getWidth(), texture.getHeight());
	LOG_WARN("Loaded Texture: [width = {0}, height = {1}]", texture.getWidth(), texture.getHeight());

	// Create a new entity -- for test
	pRegistry = std::make_unique<Registry>();
	if (!pRegistry)
	{
		LOG_ERROR("Failed to create a entt registry!");
	}
	Entity ent1{ *pRegistry,"Ent1","Test" };

	auto& transform = ent1.AddComponent<TransformComponent>(TransformComponent{
				.position = glm::vec2{10.f, 10.f},
				.scale = glm::vec2{3.f, 3.f},
				.rotation = 0.f
		}
	);
	auto& sprite = ent1.AddComponent<SpriteComponent>(SpriteComponent{
				.width = 16.f,
				.height = 16.f,
				.color = Color{.r = 255, .g = 0, .b = 255, .a = 255},
				.start_x = 1,
				.start_y = 2,
				.layer = 0,
				.texture_name = "texture1"
		}
	);

	sprite.generate_uvs(texture.getWidth(), texture.getHeight());



	auto& id = ent1.GetComponent<Identification>();
	LOG_INFO("Name :{0}, GROUP :{1},ID: {2}", id.name, id.group, id.entity_id);

	//Create the lua state
	auto lua = std::make_shared<sol::state>();
	if (!lua)
	{
		LOG_ERROR("Failed to create the lua state!");
		return false;
	}
	lua->open_libraries(
		sol::lib::base,
		sol::lib::math,
		sol::lib::os,
		sol::lib::table,
		sol::lib::io,
		sol::lib::string
	);
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

	if (!scriptSystem->LoadMainScript(*lua))
	{
		LOG_ERROR("Failed to load the main lua script");
	}
	if (!pRegistry->AddToContext<std::shared_ptr<ScriptingSystem>>(scriptSystem))
	{
		LOG_ERROR("Failed to add the scriptSystem to the registry context");
		return false;
	}

	//Setting up the rendersystem:
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

	// Create a temp camera
	auto camera = std::make_shared<Camera2D>();

	if (!pRegistry->AddToContext<std::shared_ptr<AssetManager>>(assetManager))
	{
		LOG_ERROR("Failed to add the asset manager to the registry context!");
		return false;
	}
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
	return true;
}

bool Application::LoadShaders()
{
	auto& assetManager = pRegistry->GetContext<std::shared_ptr<AssetManager>>();

	if (!assetManager)
	{
		LOG_ERROR("Failed to get the asset manager from the registry context");
		return false;
	}
	if (!assetManager->AddShader("shader1", "ASSETS/SHADER/shader.vert"
		, "ASSETS/SHADER/shader.frag"))
	{
		LOG_ERROR("Failed to add shader!");
		return false;
	}
	return true;
}

void Application::ProcessEvents()
{
}

void Application::Update()
{
	auto view = pRegistry->GetRegistry().view<TransformComponent, SpriteComponent>();

	auto& camera = pRegistry->GetContext<std::shared_ptr<Camera2D>>();
	if (!camera)
	{
		LOG_ERROR("Failed to get the camera from the registry context!");
		return;
	}

	camera->Update();

	auto& scriptSystem = pRegistry->GetContext<std::shared_ptr<ScriptingSystem>>();
	scriptSystem->Update();

	//TRYING SOMETHING FUN!
	static float rotation{ 0.f };
	static float x_pos{ 10.f };
	static bool bMoveRight{ true };
	if (rotation >= 360.f)
		rotation = 0.f;
	if (bMoveRight && x_pos < 300.f)
		x_pos += 3;
	else if (bMoveRight && x_pos >= 300.f)
		bMoveRight = false;
	if (!bMoveRight && x_pos > 10.f)
		x_pos -= 3;
	else if (!bMoveRight && x_pos <= 10.f)
		bMoveRight = true;
	for (const auto& entity : view)
	{
		Entity ent{ *pRegistry, entity };
		auto& transform = ent.GetComponent<TransformComponent>();
		transform.rotation = rotation;
		transform.position.x = x_pos;
	}
	rotation += bMoveRight ? 9 : -9;
}

void Application::Render()
{
	auto& renderSystem = pRegistry->GetContext<std::shared_ptr<RenderSystem>>();


	auto& scriptSystem = pRegistry->GetContext<std::shared_ptr<ScriptingSystem>>();
	scriptSystem->Render();


	renderSystem->Update();
}

void Application::CleanUp()
{
	Window::cleanup();
}
