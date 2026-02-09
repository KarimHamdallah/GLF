#pragma once
#include <stdint.h>

class GLindexbuffer
{
public:
	GLindexbuffer() = default;
	~GLindexbuffer();

	uint32_t GetBufferId() const { return m_Id; }
	uint32_t GetCount() const { return m_Count; }

	void Create(const void* data, uint32_t sizeinbytes);
private:
	uint32_t m_Id = 0;
	uint32_t m_Count = 0;
};
