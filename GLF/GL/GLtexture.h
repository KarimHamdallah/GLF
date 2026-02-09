#pragma once
#include <filesystem>

class GLtexture
{
public:
	GLtexture() = default;
	~GLtexture();

	uint32_t GetTextureId() const { return m_Id; }

	bool Create(const std::filesystem::path& texturepath);
	void Bind(uint32_t bindingpoint = 0) const;
private:
	uint32_t m_Id = 0;
	uint32_t m_Width, m_Height = 0;
};
