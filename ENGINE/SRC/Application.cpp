#include "Application.h"

Application& Application::GetInstance()
{
	static Application app{};
	return app;
}

Application::Application()
	:
	pRegistry{ nullptr }, m_bIsRunning{ true }
	, VAO{ 0 }, VBO{ 0 }, IBO{ 0 }// TEMP remove later
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
				.scale = glm::vec2{1.f, 1.f},
				.rotation = 0.f
		}
	);
	auto& sprite = ent1.AddComponent<SpriteComponent>(SpriteComponent{
				.width = 16.f,
				.height = 16.f,
				.color = Color{.r = 255, .g = 0, .b = 255, .a = 255},
				.start_x = 1,
				.start_y = 2
		}
	);

	sprite.generate_uvs(texture.getWidth(), texture.getHeight());

	std::vector<Vertex> vertices{};
	Vertex vTL{}, vTR{}, vBL{}, vBR{};

	vTL.position = glm::vec2{ transform.position.x, transform.position.y + sprite.height };
	vTL.uvs = glm::vec2{ sprite.uvs.u, sprite.uvs.v + sprite.uvs.uv_height };
	vTR.position = glm::vec2{ transform.position.x + sprite.width, transform.position.y + sprite.height };
	vTR.uvs = glm::vec2{ sprite.uvs.u + sprite.uvs.uv_width, sprite.uvs.v + sprite.uvs.uv_height };
	vBL.position = glm::vec2{ transform.position.x, transform.position.y };
	vBL.uvs = glm::vec2{ sprite.uvs.u, sprite.uvs.v };
	vBR.position = glm::vec2{ transform.position.x + sprite.width, transform.position.y };
	vBR.uvs = glm::vec2{ sprite.uvs.u + sprite.uvs.uv_width, sprite.uvs.v };
	vertices.push_back(vTL);
	vertices.push_back(vBL);
	vertices.push_back(vBR);
	vertices.push_back(vTR);

	auto& id = ent1.GetComponent<Identification>();
	LOG_INFO("Name :{0}, GROUP :{1},ID: {2}", id.name, id.group, id.entity_id);

	GLuint indices[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	// Let's generate the VAO
	glGenVertexArrays(1, &VAO);

	// Generate the VBO
	glGenBuffers(1, &VBO);

	// Bind the VAO and VBO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(
		GL_ARRAY_BUFFER,										// The target buffer type
		vertices.size() * sizeof(Vertex),		// The size in bytes of the buffer object's new data store
		vertices.data(),										// A pointer to the data that will be copied into the data store
		GL_STATIC_DRAW											// The expected usage pattern of the data store
	);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,								// The target buffer type
		6 * sizeof(GLuint),										// The size in bytes of the buffer object's new data store
		indices,												// A pointer to the data that will be copied into the data store
		GL_STATIC_DRAW											// The expected usage pattern of the data store
	);

	glVertexAttribPointer(
		0,														// Attribute 0	-- The layout position in the shader
		2,														// Size			-- Number of components per vertex
		GL_FLOAT,												// Type			-- The data type of the above components
		GL_FALSE,												// Normalized	-- Specifies if fixed-point data values should be normalized
		sizeof(Vertex),						// Stride		-- Specifies the byte offset between consecutive attributes
		(void*)offsetof(Vertex, position)		// Pointer		-- Specifies the offset of the first component
	);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)offsetof(Vertex, uvs)			// This the offset of the positional data to the first UV coordinate
	);

	glEnableVertexAttribArray(1);

	glVertexAttribPointer(
		2,
		4,
		GL_UNSIGNED_BYTE,
		GL_TRUE,
		sizeof(Vertex),
		(void*)offsetof(Vertex, color)			// This the offset of the positional data to the first UV coordinate
	);

	glBindVertexArray(0);

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

	// Create a temp camera
	auto camera = std::make_shared<Camera2D>();
	camera->SetScale(15.f);
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
	auto& camera = pRegistry->GetContext<std::shared_ptr<Camera2D>>();
	if (!camera)
	{
		LOG_ERROR("Failed to get the camera from the registry context!");
		return;
	}

	camera->Update();

	auto& scriptSystem = pRegistry->GetContext<std::shared_ptr<ScriptingSystem>>();
	scriptSystem->Update();
}

void Application::Render()
{
	auto& assetManager = pRegistry->GetContext<std::shared_ptr<AssetManager>>();
	auto& camera = pRegistry->GetContext<std::shared_ptr<Camera2D>>();
	auto& shader = assetManager->GetShader("shader1");
	auto projection = camera->GetCameraMatrix();

	shader.use();
	glBindVertexArray(VAO);
	shader.setMat4("uProjection", projection);
	glActiveTexture(GL_TEXTURE0);
	auto texture = assetManager->GetTexture("texture1");
	texture.bind();

	auto& scriptSystem = pRegistry->GetContext<std::shared_ptr<ScriptingSystem>>();
	scriptSystem->Render();

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
	texture.unbind();
	shader.unuse();
}

void Application::CleanUp()
{
	Window::cleanup();
}
