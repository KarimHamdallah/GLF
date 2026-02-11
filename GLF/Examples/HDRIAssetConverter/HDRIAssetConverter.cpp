#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/GLshader.h>
#include <GL/GLtexture.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <Utils/tinyfiledialogs.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <Utils/stb_image_write.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

static float s_CubeVertices[] =
{
    -1,-1,-1,  1,-1,-1,  1, 1,-1,  1, 1,-1, -1, 1,-1, -1,-1,-1,
    -1,-1, 1,  1,-1, 1,  1, 1, 1,  1, 1, 1, -1, 1, 1, -1,-1, 1,
    -1, 1, 1, -1, 1,-1, -1,-1,-1, -1,-1,-1, -1,-1, 1, -1, 1, 1,
     1, 1, 1,  1, 1,-1,  1,-1,-1,  1,-1,-1,  1,-1, 1,  1, 1, 1,
    -1,-1,-1,  1,-1,-1,  1,-1, 1,  1,-1, 1, -1,-1, 1, -1,-1,-1,
    -1, 1,-1,  1, 1,-1,  1, 1, 1,  1, 1, 1, -1, 1, 1, -1, 1,-1,
};

static const char* s_FaceNames[] = { "+X", "-X", "+Y", "-Y", "+Z", "-Z" };

static glm::mat4 s_CaptureViews[] =
{
    glm::lookAt(glm::vec3(0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0)), // +X
    glm::lookAt(glm::vec3(0), glm::vec3(0, 0,-1), glm::vec3(0, 1, 0)), // -X
    glm::lookAt(glm::vec3(0), glm::vec3(0, 1, 0), glm::vec3(-1, 0, 0)), // +Y
    glm::lookAt(glm::vec3(0), glm::vec3(0,-1, 0), glm::vec3(1, 0, 0)), // -Y
    glm::lookAt(glm::vec3(0), glm::vec3(-1, 0, 0), glm::vec3(0, 1, 0)), // +Z
    glm::lookAt(glm::vec3(0), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0)), // -Z
};

struct CubemapFace
{
    uint32_t fbo = 0;
    uint32_t colorTexture = 0;
};

// ============================================================
void SaveFaceToFile(uint32_t texture, uint32_t resolution, const char* faceName)
{
    const char* filters[] = { "*.hdr", "*.HDR" };
    std::string defaultName = std::string("face_") + faceName + ".hdr";

    const char* path = tinyfd_saveFileDialog(
        "Save Face", defaultName.c_str(), 2, filters, "HDR Image"
    );
    if (!path) return;

    std::vector<float> pixels(resolution * resolution * 3);
    glGetTextureImage(texture, 0, GL_RGB, GL_FLOAT,
        (GLsizei)(pixels.size() * sizeof(float)), pixels.data());

    stbi_write_hdr(path, (int)resolution, (int)resolution, 3, pixels.data());
    std::cout << "[Saved] " << faceName << " → " << path << "\n";
}

// ============================================================
void ConvertHDRI(const std::string& hdriPath, uint32_t resolution,
    CubemapFace faces[6], uint32_t cubeVAO)
{
    GLtexture hdriTex;
    if (!hdriTex.CreateHDRI(hdriPath, true))
    {
        std::cerr << "Failed to load HDRI\n";
        return;
    }

    GLshader convShader;
    convShader.Create(
        ASSETS_DIR "HDRIAssetConverter/equirect_to_cube.vert",
        ASSETS_DIR "HDRIAssetConverter/equirect_to_cube.frag"
    );

    glm::mat4 captureProj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

    convShader.Bind();
    convShader.SetMat4("projection", captureProj);
    convShader.SetInt("equirectangularMap", 0);
    hdriTex.Bind(0);

    glViewport(0, 0, resolution, resolution);
    glBindVertexArray(cubeVAO);

    for (uint32_t i = 0; i < 6; i++)
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &faces[i].colorTexture);
        glTextureStorage2D(faces[i].colorTexture, 1, GL_RGB16F, resolution, resolution);
        glTextureParameteri(faces[i].colorTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(faces[i].colorTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glCreateFramebuffers(1, &faces[i].fbo);
        glNamedFramebufferTexture(faces[i].fbo, GL_COLOR_ATTACHMENT0,
            faces[i].colorTexture, 0);

        uint32_t rbo;
        glCreateRenderbuffers(1, &rbo);
        glNamedRenderbufferStorage(rbo, GL_DEPTH_COMPONENT24, resolution, resolution);
        glNamedFramebufferRenderbuffer(faces[i].fbo, GL_DEPTH_ATTACHMENT,
            GL_RENDERBUFFER, rbo);
        glDeleteRenderbuffers(1, &rbo);

        glBindFramebuffer(GL_FRAMEBUFFER, faces[i].fbo);
        convShader.SetMat4("view", s_CaptureViews[i]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        std::cout << "[Face " << s_FaceNames[i] << "] Done\n";
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    std::cout << "[ConvertHDRI] All 6 faces rendered!\n";
}

// ============================================================
int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(900, 750, "HDRI Converter", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glEnable(GL_DEPTH_TEST);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
    ImGui::StyleColorsDark();

    uint32_t cubeVAO, cubeVBO;
    glCreateVertexArrays(1, &cubeVAO);
    glCreateBuffers(1, &cubeVBO);
    glNamedBufferData(cubeVBO, sizeof(s_CubeVertices), s_CubeVertices, GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(cubeVAO, 0, cubeVBO, 0, 3 * sizeof(float));
    glEnableVertexArrayAttrib(cubeVAO, 0);
    glVertexArrayAttribFormat(cubeVAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(cubeVAO, 0, 0);

    std::string selectedHDRIPath = "";
    bool        converted = false;
    uint32_t    resolution = 512;
    CubemapFace faces[6] = {};

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowSize({ 870, 700 }, ImGuiCond_Once);
        ImGui::SetNextWindowPos({ 15, 15 }, ImGuiCond_Once);
        ImGui::Begin("HDRI Converter");

        // ── Browse ──
        if (ImGui::Button("Browse HDRI File..."))
        {
            const char* filters[] = { "*.hdr", "*.HDR" };
            const char* path = tinyfd_openFileDialog(
                "اختار ملف HDRI", "", 2, filters, "HDR Files", 0
            );
            if (path) { selectedHDRIPath = path; converted = false; }
        }
        ImGui::SameLine();
        if (!selectedHDRIPath.empty())
            ImGui::TextColored({ 0.4f, 1.0f, 0.4f, 1.0f }, "%s", selectedHDRIPath.c_str());
        else
            ImGui::TextDisabled("لم يتم اختيار ملف");

        ImGui::Separator();

        // ── Resolution ──
        ImGui::Text("Resolution:"); ImGui::SameLine();
        const char* resOptions[] = { "256", "512", "1024", "2048" };
        int resIndex = (resolution == 256) ? 0 : (resolution == 512) ? 1 :
            (resolution == 1024) ? 2 : 3;
        if (ImGui::Combo("##res", &resIndex, resOptions, 4))
        {
            resolution = 256 << resIndex;
            converted = false;
        }

        ImGui::Separator();

        // ── Convert Button ──
        bool canConvert = !selectedHDRIPath.empty() && !converted;
        if (!canConvert) ImGui::BeginDisabled();
        if (ImGui::Button("  Convert to Cubemap Faces  "))
        {
            for (auto& f : faces)
            {
                if (f.fbo)          glDeleteFramebuffers(1, &f.fbo);
                if (f.colorTexture) glDeleteTextures(1, &f.colorTexture);
                f = {};
            }
            ConvertHDRI(selectedHDRIPath, resolution, faces, cubeVAO);
            converted = true;
        }
        if (!canConvert) ImGui::EndDisabled();

        ImGui::Separator();

        // ── Preview ──
        if (converted)
        {
            ImGui::Text("Cubemap Faces:  ");
            ImGui::SameLine();
            ImGui::TextDisabled("(click any face to save as .hdr)");
            ImGui::Spacing();

            float size = 150.0f;
            float padding = 2.0f;

            // helper lambda
            auto DrawFace = [&](int idx, const char* label)
                {
                    ImGui::BeginGroup();
                    ImGui::Text("%s", label);
                    ImGui::Image(
                        (ImTextureID)(intptr_t)faces[idx].colorTexture,
                        { size, size }, { 0,1 }, { 1,0 }
                    );
                    if (ImGui::IsItemClicked())
                        SaveFaceToFile(faces[idx].colorTexture, resolution, label);
                    if (ImGui::IsItemHovered())
                        ImGui::SetTooltip("Click to save %s", label);
                    ImGui::EndGroup();
                };

            // صف 1: فراغ + +Y
            ImGui::Dummy({ size, size });
            ImGui::SameLine(0, padding);
            DrawFace(2, "top +Y");

            ImGui::Spacing();

            // صف 2: -X، -Z، +X، +Z
            DrawFace(1, "left -X");   ImGui::SameLine(0, padding);
            DrawFace(5, "front -Z");  ImGui::SameLine(0, padding);
            DrawFace(0, "right +X");  ImGui::SameLine(0, padding);
            DrawFace(4, "back +Z");

            ImGui::Spacing();

            // صف 3: فراغ + -Y
            ImGui::Dummy({ size, size });
            ImGui::SameLine(0, padding);
            DrawFace(3, "bottom -Y");
        }

        ImGui::End();

        // ── Render ──
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);
        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // ── Cleanup ──
    for (auto& f : faces)
    {
        if (f.fbo)          glDeleteFramebuffers(1, &f.fbo);
        if (f.colorTexture) glDeleteTextures(1, &f.colorTexture);
    }
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
