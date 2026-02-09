#pragma once
#include <stdint.h>

class GLvertexbuffer
{
public:
	GLvertexbuffer() = default;
	~GLvertexbuffer();

	uint32_t GetBufferId() const { return m_Id; }

	void Create(const void* data, uint32_t sizeinbytes);
private:
	uint32_t m_Id = 0;
};
