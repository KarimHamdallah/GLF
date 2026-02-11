#pragma once
#include <filesystem>
#include <vector>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <GL/GLvertexarray.h>
#include <GL/GLtexture.h>

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct MeshEntry
{
    uint32_t indexCount;
    uint32_t indexOffset;
    uint32_t baseVertex;

    int AlbedoTextureIndex = -1;  // index in m_Textures, -1 = no texture
};

struct DrawCommand
{
    uint32_t indexCount;
    uint32_t instanceCount; // = 1
    uint32_t indexOffset;
    uint32_t baseVertex;
    uint32_t baseInstance;  // = 0
};

class GLmodel
{
public:
    GLmodel() = default;
    void Load(const std::filesystem::path& path, float scale = 1.0f);
    void Draw();

private:
    void processNode(aiNode* node, const aiScene* scene, glm::mat4 parentTransform,
        std::vector<Vertex>& allVertices,
        std::vector<uint32_t>& allIndices);

    int LoadTextureIfIsntLoaded(const std::string& path);
private:
    std::filesystem::path m_Path;
    float m_Scale = 1.0f;

    GLvertexarray m_VAO;
    GLvertexbuffer m_VBO;
    GLindexbuffer  m_EBO;

    std::vector<MeshEntry> m_Meshes;
    std::vector<GLtexture> m_Textures;
    std::unordered_map<std::string, uint32_t> m_TextureCache;
    uint32_t m_CommandBufferID = 0;
    uint32_t m_TextureBufferID = 0;
};
