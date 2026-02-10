#pragma once
#include <filesystem>
#include <vector>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <GL/GLvertexarray.h>

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
    void Load(const std::filesystem::path& path);
    void Draw();

private:
    void processNode(aiNode* node, const aiScene* scene, glm::mat4 parentTransform,
        std::vector<Vertex>& allVertices,
        std::vector<uint32_t>& allIndices);
private:
    std::filesystem::path m_Path;

    GLvertexarray m_VAO;
    GLvertexbuffer m_VBO;
    GLindexbuffer  m_EBO;

    std::vector<MeshEntry> m_Meshes;
    uint32_t m_CommandBufferID = 0;
};
