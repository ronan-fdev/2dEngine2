#include <iostream>

#include "../WINDOW/Window.h"
#include "../LOGGER/log.h"
#include "../RENDERER/TEXTURE/TextureLoader.h"
#include "../RENDERER/SHADER/ShaderLoader.h"
#include "../RENDERER/CORE/Camera2D.h"
#include "../RENDERER/ESSENTIALS/Vertex.h"
#include "../ECS/Entity.h";
#include "../ECS/COMPONENTS/SpriteComponent.h";
#include "../ECS/COMPONENTS/TransformComponent.h";
#include "../ECS/COMPONENTS/Identification.h"

#include <glm/glm.hpp>


int main()
{
	std::cout << "Hello World!" << std::endl;

	//Logger
	log::Init();

	Window::init(640, 480, "2DENGINE");
	

	auto texture = TextureLoader::createTexture("ASSETS/IMAGES/spritesheet.png");
	if (!texture)
	{
		LOG_ERROR("Failed to create a texture!");
		return -1;
	}
	// Let's make some temporary UVs
	UVs uVs{};
	LOG_INFO("Loaded Texture: [width = {0}, height = {1}]", texture->getWidth(), texture->getHeight());
	LOG_WARN("Loaded Texture: [width = {0}, height = {1}]", texture->getWidth(), texture->getHeight());

	// Create a new entity -- for test
	//auto ent1 = pRegistry->create();
	auto pRegistry = std::make_unique<Registry>();
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

	sprite.generate_uvs(texture->getWidth(), texture->getHeight());

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
	// Now we will have to create the vertex array object and the vertex buffer object
	GLuint VAO, VBO, IBO;

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

	auto shader = ShaderLoader::createShader("ASSETS/SHADER/shader.vert"
		, "ASSETS/SHADER/shader.frag");
	if (!shader)
	{
		LOG_ERROR("Failed to create the shader!");
		return -1;
	}

	Camera2D camera{};
	camera.SetScale(15.f);

	while (!Window::shouldClose())
	{
		Window::clearScreen();
		Window::processInput();
		if (Window::getdt() > 0.0f)
		{
			shader->use();
			glBindVertexArray(VAO);
			
			auto projection = camera.GetCameraMatrix();
			shader->setMat4("uProjection", projection);

			glActiveTexture(GL_TEXTURE0);
			texture->bind();

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

			glBindVertexArray(0);

			camera.Update();
			shader->unuse();
		}
		Window::update();
	}
	Window::cleanup();
	return 0;
}
