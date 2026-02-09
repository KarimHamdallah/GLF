#include "GLvertexbuffer.h"
#include <glad/glad.h>

GLvertexbuffer::~GLvertexbuffer()
{
	if(m_Id)
		glDeleteBuffers(1, &m_Id);
}

void GLvertexbuffer::Create(const void* data, uint32_t sizeinbytes)
{
	glCreateBuffers(1, &m_Id);
	glNamedBufferData(m_Id, sizeinbytes, data, GL_STATIC_DRAW);
}
