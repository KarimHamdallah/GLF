#include "GLvertexarray.h"
#include <glad/glad.h>

namespace
{
	GLenum ShaderAttribDataTypeToGL(ShaderAttribDataType type)
	{
		switch (type)
		{
		case FLOAT: return GL_FLOAT;
		case INT: return GL_INT;
		default: return GL_FLOAT;
		}
	}
}

GLvertexarray::~GLvertexarray()
{
	if (m_Id)
		glDeleteVertexArrays(1, &m_Id);
}

void GLvertexarray::Create()
{
	glCreateVertexArrays(1, &m_Id);
}

void GLvertexarray::AddVertexBuffer(const GLvertexbuffer& vertexbuffer, uint32_t bindingindex, uint32_t stride)
{
	glVertexArrayVertexBuffer(m_Id, bindingindex, vertexbuffer.GetBufferId(), 0, stride);
}

void GLvertexarray::PushAttrib(uint32_t shaderlocation, int count, ShaderAttribDataType type, uint32_t bindingindex)
{
	glVertexArrayAttribFormat(m_Id, shaderlocation, count, ShaderAttribDataTypeToGL(type), GL_FALSE, m_OffsetAccumlator * sizeof(float));
	glEnableVertexArrayAttrib(m_Id, shaderlocation);
	glVertexArrayAttribBinding(m_Id, shaderlocation, bindingindex);

	m_OffsetAccumlator += count;
}

void GLvertexarray::Bind() const
{
	glBindVertexArray(m_Id);
}
