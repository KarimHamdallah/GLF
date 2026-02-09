#pragma once
#include "GLvertexbuffer.h"
#include "GLindexbuffer.h"

enum ShaderAttribDataType { FLOAT, INT };

class GLvertexarray
{
public:
	GLvertexarray() = default;
	~GLvertexarray();

	void Create();
	void AddVertexBuffer(const GLvertexbuffer& vertexbuffer, uint32_t bindingindex, uint32_t stride);
	void AddIndexBuffer(const GLindexbuffer& indexbuffer);
	void PushAttrib(uint32_t shaderlocation, int count, ShaderAttribDataType type, uint32_t bindingindex);

	void Bind() const;

private:
	uint32_t m_Id = 0;
	uint32_t m_OffsetAccumlator = 0;
};
