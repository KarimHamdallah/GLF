#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/GLshader.h>
#include <GL/GLvertexarray.h>

int main()
{
	glfwInit();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "GLF - Hello Triangle", nullptr, nullptr);

	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	// create triangle shader
	GLshader SimpleTriangleShader;
	SimpleTriangleShader.Create(
		ASSETS_DIR "simple_triangle.vert",
		ASSETS_DIR "simple_triangle.frag"
	);


	// draw triangle

	GLfloat vertices[] = {
		// positions                // color
		 0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f, 1.0f,   // bottom right
		 0.0f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f, 1.0f,   // top
		-0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f, 1.0f    // bottom left
	};

	GLvertexbuffer vertexbuffer;
	vertexbuffer.Create(vertices, sizeof(vertices));

	GLvertexarray vertexarray;
	vertexarray.Create();
	vertexarray.AddVertexBuffer(vertexbuffer, 0, 7 * sizeof(GLfloat));
	vertexarray.PushAttrib(0, 3, ShaderAttribDataType::FLOAT, 0);
	vertexarray.PushAttrib(1, 4, ShaderAttribDataType::FLOAT, 0);


	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();


		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		SimpleTriangleShader.Bind();
		vertexarray.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);


		glfwSwapBuffers(window);
	}

	return 0;
}
