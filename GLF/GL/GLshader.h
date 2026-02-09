#pragma once
#include <stdint.h>
#include <filesystem>

class GLshader
{
public:
	GLshader() = default;
	bool Create(const std::filesystem::path& vertexshaderpath, const std::filesystem::path& fragmentshaderpath);
	~GLshader();

	uint32_t GetShaderId() const { return m_Id; }
	void Bind();
private:
	std::pair<uint32_t, uint32_t> CompileShaders(const std::string& vertexshadersrc, const std::string& fragmentshadersrc);
	void LinkShaders(uint32_t vertex, uint32_t fragment);
private:
	uint32_t m_Id = 0;
};
