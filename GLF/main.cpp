#include <iostream>
#include <GLFW/glfw3.h>
#include <Math/Math.h>


int main()
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello window", nullptr, nullptr);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}

	return 0;
}
