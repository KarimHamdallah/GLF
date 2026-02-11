#pragma once
#include <filesystem>

class GLtexture
{
public:
	GLtexture() = default;
	GLtexture(GLtexture&& other) noexcept;
	~GLtexture();

	uint32_t GetTextureId() const { return m_Id; }
	uint64_t GetBindlessHandle() const { return m_BindlessHandle; }

	bool Create(const std::filesystem::path& texturepath);
	void Bind(uint32_t bindingpoint = 0) const;
private:
	uint32_t m_Id = 0;
	uint64_t m_BindlessHandle = 0;
	uint32_t m_Width, m_Height = 0;
};
