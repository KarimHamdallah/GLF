/*
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/GLshader.h>
#include <GL/GLvertexarray.h>
#include <GL/GLtexture.h>
#include <GL/Transform.h>
#include <Core/Input.h>
#include <GL/Camera3D.h>
#include <GL/GLmodel.h>

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

	glEnable(GL_DEPTH_TEST);

	Input::Init(window);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// create triangle shader
	GLshader ModelShader;
	ModelShader.Create(
		ASSETS_DIR "3D Model Loading/model.vert",
		ASSETS_DIR "3D Model Loading/model.frag"
	);

	GLmodel model;
	model.Load(ASSETS_DIR "3D Model Loading/car/scene.gltf", 0.01f);

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//transform.m_Rotation.y += 3.0f * deltaTime;

		ModelShader.Bind();
		ModelShader.SetMat4("view_proj", proj * Camera.GetViewMatrix());
		ModelShader.SetMat4("model", transform.GetTransformationMatrix());

		model.Draw();


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
*/