#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/GLshader.h>
#include <GL/GLvertexarray.h>
#include <GL/GLtexture.h>
#include <GL/Transform.h>
#include <Core/Input.h>
#include <GL/Camera3D.h>

uint32_t Window_Width = 800;
uint32_t Window_Height = 600;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
float LastMouseX, LastMouseY = 0;
bool FirstMouse = true;


int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(Window_Width, Window_Height, "GLF - Sprite", nullptr, nullptr);

	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	Input::Init(window);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

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

	Camera3D Camera;
	Camera.Init({ 0.0f, 0.0f, 3.0f });
	
	glm::mat4 proj = glm::perspective(glm::radians(Camera.GetFov()), (float)Window_Width / (float)Window_Height, Camera.GetNearPlane(), Camera.GetFarPlane());

	TransformComponent transform;
	transform.m_Position.z = -5.0f;



	while (!glfwWindowShouldClose(window))
	{
		// ========== Delta Time Calculation ==========
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		Input::ResetInput();

		glfwPollEvents();


		Camera.Update(deltaTime);


		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		transform.m_Rotation.y += 3.0f * deltaTime;

		SpriteShader.Bind();
		SpriteShader.SetInt("Texture0", 0);
		SpriteShader.SetMat4("view_proj", proj * Camera.GetViewMatrix());
		SpriteShader.SetMat4("model", transform.GetTransformationMatrix());

		texture.Bind();
		vertexarray.Bind();
		glDrawElements(GL_TRIANGLES, indexbuffer.GetCount(), GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);
	}

	return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	// Mouse Pos
	Input::SetMousePosition({ (float)xpos, (float)ypos });

	// Mouse Offset
	if (FirstMouse)
	{
		LastMouseX = xpos;
		LastMouseY = ypos;
		FirstMouse = false;
	}

	float xoffset = xpos - LastMouseX;
	float yoffset = LastMouseY - ypos; // „⁄ﬂÊ”… ·√‰ ≈Õœ«ÀÌ«  Y  »œ√ „‰ «·√⁄·Ï ··√”›·

	Input::SetMouseOffset({ xoffset, yoffset });

	LastMouseX = xpos;
	LastMouseY = ypos;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// Mouse Scroll
	Input::SetMouseScroll({ (float)xoffset, (float)yoffset });
}
