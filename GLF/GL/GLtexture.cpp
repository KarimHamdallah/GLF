#include "GLtexture.h"
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <Utils/stb_image.h>
#include <iostream>

GLtexture::~GLtexture()
{
	if (m_Id)
	{
        glDeleteTextures(1, &m_Id);
	}
}

bool GLtexture::Create(const std::filesystem::path& texturepath)
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true); // Flip texture vertically if needed
    unsigned char* data = stbi_load(texturepath.string().c_str(), &width, &height, &channels, 0);

    if (!data)
    {
        std::cerr << "Failed to load texture: " << texturepath.string().c_str() << std::endl;
        return false;
    }

    m_Width = width;
    m_Height = height;

    GLenum internalFormat = GL_RGB8;
    GLenum dataFormat = GL_RGB;

    if (channels == 4)
    {
        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;
    }
    else if (channels == 3)
    {
        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;
    }
    else
    {
        std::cerr << "Unsupported number of channels: " << channels << std::endl;
        stbi_image_free(data);
        return false;
    }


    glCreateTextures(GL_TEXTURE_2D, 1, &m_Id);
    glTextureStorage2D(m_Id, 1, internalFormat, width, height);
    glTextureSubImage2D(m_Id, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);

    glTextureParameteri(m_Id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_Id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(m_Id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_Id, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGenerateTextureMipmap(m_Id);

    stbi_image_free(data);

    return true;
}

void GLtexture::Bind(uint32_t bindingpoint) const
{
    glBindTextureUnit(bindingpoint, m_Id);
}
