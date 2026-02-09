# GLF

![GLF Logo](githubcontent/glf.png)

Graphics Framework For Testing And Learning Computer Graphics Programming

## About

GLF is a simple, modern OpenGL framework designed for learning computer graphics. It provides clean abstractions over core OpenGL objects (Shaders, VAO, VBO) making it easier to focus on graphics concepts rather than boilerplate code.

### Features
- 🎨 Clean, intuitive API
- ⚡ Modern OpenGL 4.6 Core Profile
- 📚 Learning-focused design
- 🛠️ Easy to understand and extend

## Quick Start
```bash
# Clone and build
git clone https://github.com/KarimHamdallah/GLF.git
cd GLF && mkdir build && cd build
cmake .. && cmake --build .

# Run
./GLF
```

## Examples

### Hello Triangle

![Hello Triangle](githubcontent/glf_hello_triangle.png)

Basic rendering with vertex colors

**Code:**
```cpp
// Create shader
GLshader SimpleTriangleShader;
SimpleTriangleShader.Create(
    ASSETS_DIR "simple_triangle.vert",
    ASSETS_DIR "simple_triangle.frag"
);

// Define vertices
GLfloat vertices[] = {
    // positions            // colors
     0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f, 1.0f,   // bottom right
     0.0f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f, 1.0f,   // top
    -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f, 1.0f    // bottom left
};

// Setup buffers
GLvertexbuffer vertexbuffer;
vertexbuffer.Create(vertices, sizeof(vertices));

GLvertexarray vertexarray;
vertexarray.Create();
vertexarray.AddVertexBuffer(vertexbuffer, 0, 7 * sizeof(GLfloat));
vertexarray.PushAttrib(0, 3, ShaderAttribDataType::FLOAT, 0);
vertexarray.PushAttrib(1, 4, ShaderAttribDataType::FLOAT, 0);

// Render
SimpleTriangleShader.Bind();
vertexarray.Bind();
glDrawArrays(GL_TRIANGLES, 0, 3);

// glsl

#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
```

**Made with ❤️ for learning graphics programming**