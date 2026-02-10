#include "GLmodel.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <iostream>

namespace
{
    glm::mat4 aiToGlm(const aiMatrix4x4& m)
    {
        glm::mat4 r;
        r[0][0] = m.a1; r[1][0] = m.a2; r[2][0] = m.a3; r[3][0] = m.a4;
        r[0][1] = m.b1; r[1][1] = m.b2; r[2][1] = m.b3; r[3][1] = m.b4;
        r[0][2] = m.c1; r[1][2] = m.c2; r[2][2] = m.c3; r[3][2] = m.c4;
        r[0][3] = m.d1; r[1][3] = m.d2; r[2][3] = m.d3; r[3][3] = m.d4;
        return r;
    }
}

void GLmodel::Load(const std::filesystem::path& path)
{
    Assimp::Importer importer;

    auto flags = 
        aiProcess_Triangulate |
        aiProcess_FlipUVs |
        aiProcess_GenNormals;

    const aiScene* scene = importer.ReadFile(path.string(), flags);

    if (!scene || !scene->mRootNode)
    {
        std::cerr << "Failed To Load Model At Path: " << path.string() << "\nError Message: " << importer.GetErrorString() << "\n";
        return;
    }
    
    m_Path = path;

    std::vector<Vertex> allVertices;
    std::vector<uint32_t> allIndices;

    // fill vertices and indices buffers
    processNode(scene->mRootNode, scene, glm::mat4(1.0f), allVertices, allIndices);

    // upload to gpu
    m_VBO.Create(allVertices.data(), allVertices.size() * sizeof(Vertex));
    m_EBO.Create(allIndices.data(), allIndices.size() * sizeof(uint32_t));

    m_VAO.Create();
    m_VAO.AddVertexBuffer(m_VBO, 0, sizeof(Vertex));
    m_VAO.AddIndexBuffer(m_EBO);

    m_VAO.PushAttrib(0, 3, ShaderAttribDataType::FLOAT, 0);
    m_VAO.PushAttrib(1, 3, ShaderAttribDataType::FLOAT, 0);
    m_VAO.PushAttrib(2, 2, ShaderAttribDataType::FLOAT, 0);

    // upload command buffer
    std::vector<DrawCommand> commands;
    for (auto& mesh : m_Meshes)
    {
        DrawCommand cmd;
        cmd.indexCount = mesh.indexCount;
        cmd.instanceCount = 1;
        cmd.indexOffset = mesh.indexOffset;
        cmd.baseVertex = mesh.baseVertex;
        cmd.baseInstance = 0;
        commands.push_back(cmd);
    }

    glCreateBuffers(1, &m_CommandBufferID);
    glNamedBufferData(m_CommandBufferID, commands.size() * sizeof(DrawCommand), commands.data(), GL_STATIC_DRAW);
}

void GLmodel::Draw()
{
    m_VAO.Bind();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    /*
    for (auto& mesh : m_Meshes)
    {
        glDrawElementsBaseVertex(
            GL_TRIANGLES,
            mesh.indexCount,
            GL_UNSIGNED_INT,
            (void*)(mesh.indexOffset * sizeof(uint32_t)),
            mesh.baseVertex
        );
    }
    */

    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_CommandBufferID);
    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, m_Meshes.size(), 0);
}

void GLmodel::processNode(aiNode* node, const aiScene* scene, glm::mat4 parentTransform,
    std::vector<Vertex>& allVertices,
    std::vector<uint32_t>& allIndices)
{
    glm::mat4 globalTransform = parentTransform * aiToGlm(node->mTransformation);
    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(globalTransform)));

    for (uint32_t i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];

        MeshEntry entry;
        entry.baseVertex = (uint32_t)allVertices.size();
        entry.indexOffset = (uint32_t)allIndices.size();

        // vertices
        for (uint32_t v = 0; v < aiMesh->mNumVertices; v++)
        {
            Vertex vertex;

            glm::vec4 pos = globalTransform * glm::vec4(
                aiMesh->mVertices[v].x,
                aiMesh->mVertices[v].y,
                aiMesh->mVertices[v].z,
                1.0f
            );
            vertex.Position = glm::vec3(pos);

            if (aiMesh->HasNormals()) {
                vertex.Normal = glm::normalize(normalMatrix * glm::vec3(
                    aiMesh->mNormals[v].x,
                    aiMesh->mNormals[v].y,
                    aiMesh->mNormals[v].z
                ));
            }
            else
                std::cerr << "Model at path: " << m_Path.string() << "Has No Normals!\n";

            if (aiMesh->mTextureCoords[0])
            {
                vertex.TexCoords = glm::vec2(aiMesh->mTextureCoords[0][v].x, aiMesh->mTextureCoords[0][v].y);
            }
            else
                std::cerr << "Model at path: " << m_Path.string() << "Has No TexCoords!\n";

            allVertices.push_back(vertex);
        }

        for (uint32_t f = 0; f < aiMesh->mNumFaces; f++)
        {
            aiFace& face = aiMesh->mFaces[f];
            for (uint32_t j = 0; j < face.mNumIndices; j++)
                allIndices.push_back(face.mIndices[j]);
        }

        entry.indexCount = (uint32_t)allIndices.size() - entry.indexOffset;
        m_Meshes.push_back(entry);
    }

    for (uint32_t i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene, globalTransform, allVertices, allIndices);
}
