#include "GLshader.h"
#include <Utils/Utils.h>
#include <glad/glad.h>

bool GLshader::Create(const std::filesystem::path& vertexshaderpath, const std::filesystem::path& fragmentshaderpath)
{
	std::string VertexShaderSrc = ReadFileToString(vertexshaderpath);
	std::string FragmentShaderSrc = ReadFileToString(fragmentshaderpath);

    auto[vertex, fragment] = CompileShaders(VertexShaderSrc, FragmentShaderSrc);
    LinkShaders(vertex, fragment);

    return true;
}

GLshader::~GLshader()
{
    if (m_Id)
    {
        glDeleteProgram(m_Id);
    }
}

void GLshader::Bind()
{
    glUseProgram(m_Id);
}

std::pair<uint32_t, uint32_t> GLshader::CompileShaders(const std::string& vertexshadersrc, const std::string& fragmentshadersrc)
{
    const char* vShaderCode = vertexshadersrc.c_str();
    const char* fShaderCode = fragmentshadersrc.c_str();

    uint32_t vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    // Check Vertex Shader Compilation Errors
    {
        GLint success = 0;
        GLchar infoLog[1024] = {};
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR <VERTEX SHADER>:\n" << infoLog << "\n-- -------------------------------------------------- - -- \n";
        }
    }

    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    // Check Fragment Shader Compilation Errors
    {
        GLint success = 0;
        GLchar infoLog[1024] = {};
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR <FRAGMENT SHADER>:\n" << infoLog << "\n-- -------------------------------------------------- - -- \n";
        }
    }

    return { vertex, fragment };
}

void GLshader::LinkShaders(uint32_t vertex, uint32_t fragment)
{
    m_Id = glCreateProgram();
    glAttachShader(m_Id, vertex);
    glAttachShader(m_Id, fragment);
    glLinkProgram(m_Id);

    // Check Shader Program Linking Errors
    {
        GLint success = 0;
        GLchar infoLog[1024] = {};
        glGetProgramiv(m_Id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(m_Id, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_PROGRAM_LINKING_ERROR:\n" << infoLog << "\n-- -------------------------------------------------- - -- \n";
        }
    }

    // Delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}
