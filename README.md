# <div align="center">

# 

# <img src="docs/glf.png" alt="GLF Logo" width="300"/>

# 

# \# GLF

# 

# \*\*Graphics Framework For Testing And Learning Computer Graphics Programming\*\*

# 

# \[!\[License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

# \[!\[C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)

# \[!\[OpenGL](https://img.shields.io/badge/OpenGL-4.6-green.svg)](https://www.opengl.org/)

# 

# A lightweight C++ wrapper around OpenGL for learning graphics programming with practical examples.

# 

# </div>

# 

# ---

# 

# \## 🎯 About

# 

# GLF is a simple, modern OpenGL framework designed for learning computer graphics. It provides clean abstractions over core OpenGL objects (Shaders, VAO, VBO) making it easier to focus on graphics concepts rather than boilerplate code.

# 

# \### Features

# \- 🎨 Clean, intuitive API

# \- ⚡ Modern OpenGL 4.6 Core Profile

# \- 📚 Learning-focused design

# \- 🛠️ Easy to understand and extend

# 

# \## 🚀 Quick Start

# ```bash

# \# Clone and build

# git clone https://github.com/YourUsername/GLF.git

# cd GLF \&\& mkdir build \&\& cd build

# cmake .. \&\& cmake --build .

# 

# \# Run

# ./GLF

# ```

# 

# \## 📚 Examples

# 

# <div align="center">

# <img src="docs/examples/glf\_hello\_triangle.png" alt="Hello Triangle" width="500"/>

# 

# \*\*Hello Triangle\*\* - Basic rendering with vertex colors

# </div>

# 

# \## 🛠️ Framework API

# ```cpp

# // create triangle shader

# GLshader SimpleTriangleShader;

# SimpleTriangleShader.Create(

# &nbsp;	ASSETS\_DIR "simple\_triangle.vert",

# &nbsp;	ASSETS\_DIR "simple\_triangle.frag"

# );

# 

# 

# // draw triangle

# 

# GLfloat vertices\[] = {

# &nbsp;	// positions                // color

# &nbsp;	 0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f, 1.0f,   // bottom right

# &nbsp;	 0.0f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f, 1.0f,   // top

# &nbsp;	-0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f, 1.0f    // bottom left

# };

# 

# GLvertexbuffer vertexbuffer;

# vertexbuffer.Create(vertices, sizeof(vertices));

# 

# GLvertexarray vertexarray;

# vertexarray.Create();

# vertexarray.AddVertexBuffer(vertexbuffer, 0, 7 \* sizeof(GLfloat));

# vertexarray.PushAttrib(0, 3, ShaderAttribDataType::FLOAT, 0);

# vertexarray.PushAttrib(1, 4, ShaderAttribDataType::FLOAT, 0);

# 









# ////// Draw ///////



# SimpleTriangleShader.Bind();

# vertexarray.Bind();

# glDrawArrays(GL\_TRIANGLES, 0, 3);



//////////////// Shader /////////////

#version 460 core
===

# 

# layout (location = 0) in vec3 aPos;

# layout (location = 1) in vec4 aColor;

# 

/////////////////////////////////////

```
===

# 

# ---

# 

# <div align="center">

# 

# \*\*Made with ❤️ for learning graphics programming\*\*

# 

# </div>

# ```

# 

# ---

