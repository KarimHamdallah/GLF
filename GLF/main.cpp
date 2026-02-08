#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/GLshader.h>

int main()
{
	glfwInit();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello window", nullptr, nullptr);

	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	// create triangle shader
	GLshader SimpleTriangleShader;
	SimpleTriangleShader.Create(
		ASSETS_DIR "simple_triangle.vert",
		ASSETS_DIR "simple_triangle.frag"
	);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();


		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
	}

	return 0;
}
