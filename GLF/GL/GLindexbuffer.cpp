#include "GLindexbuffer.h"
#include <glad/glad.h>

GLindexbuffer::~GLindexbuffer()
{
	if (m_Id)
		glDeleteBuffers(1, &m_Id);
}

void GLindexbuffer::Create(const void* data, uint32_t sizeinbytes)
{
    m_Count = sizeinbytes / sizeof(uint32_t);
    glCreateBuffers(1, &m_Id);
    glNamedBufferData(m_Id, sizeinbytes, data, GL_STATIC_DRAW);
}
