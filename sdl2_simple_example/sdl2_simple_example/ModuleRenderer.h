#ifndef __ModuleRenderer_H__
#define __ModuleRenderer_H__

#include "Module.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>

class Application;

class ModuleRenderer : public Module {
public:
    ModuleRenderer(Application* app, bool start_enabled = true);
    ~ModuleRenderer();

    bool Init() override;
    bool Start() override;
    update_status PreUpdate(float dt) override;
    update_status Update(float dt) override;
    update_status PostUpdate(float dt) override;
    bool CleanUp() override;

    bool LoadModel(const char* path);

private:
    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

    struct Mesh {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        unsigned int VAO, VBO, EBO;

        void SetupMesh();
        void Draw();
    };

    std::vector<Mesh> meshes;
    std::string directory;

    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
};

#endif