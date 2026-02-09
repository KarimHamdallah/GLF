#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/GLshader.h>
#include <GL/GLvertexarray.h>
#include <GL/GLtexture.h>
#include <GL/Transform.h>

uint32_t Window_Width = 800;
uint32_t Window_Height = 600;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(Window_Width, Window_Height, "GLF - Sprite", nullptr, nullptr);

	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	// create triangle shader
	GLshader SpriteShader;
	SpriteShader.Create(
		ASSETS_DIR "Sprite2D/sprite.vert",
		ASSETS_DIR "Sprite2D/sprite.frag"
	);


	float vertices[] = 
	{
		// Positions          // Texture Coords
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f,   // Top Left
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // Bottom Left
		 0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // Bottom Right
		 0.5f,  0.5f, 0.0f,   1.0f, 1.0f    // Top Right
	};

	uint32_t indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};


	GLvertexbuffer vertexbuffer;
	vertexbuffer.Create(vertices, sizeof(vertices));

	GLindexbuffer indexbuffer;
	indexbuffer.Create(indices, sizeof(indices));

	GLvertexarray vertexarray;
	vertexarray.Create();
	vertexarray.AddVertexBuffer(vertexbuffer, 0, 5 * sizeof(GLfloat));
	vertexarray.AddIndexBuffer(indexbuffer);
	vertexarray.PushAttrib(0, 3, ShaderAttribDataType::FLOAT, 0);
	vertexarray.PushAttrib(1, 2, ShaderAttribDataType::FLOAT, 0);


	GLtexture texture;
	texture.Create(ASSETS_DIR "Sprite2D/glf.png");

	glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)Window_Width / (float)Window_Height, 0.1f, 100.0f);

	TransformComponent transform;
	transform.m_Position.z = -5.0f;



	while (!glfwWindowShouldClose(window))
	{
		// ========== Delta Time Calculation ==========
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();


		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		transform.m_Rotation.y += 3.0f * deltaTime;

		SpriteShader.Bind();
		SpriteShader.SetInt("Texture0", 0);
		SpriteShader.SetMat4("proj", proj);
		SpriteShader.SetMat4("model", transform.GetTransformationMatrix());

		texture.Bind();
		vertexarray.Bind();
		glDrawElements(GL_TRIANGLES, indexbuffer.GetCount(), GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);
	}

	return 0;
}
