#ifndef __ModuleRenderer_H__
#define __ModuleRenderer_H__

#include "Module.h"
#include "Defs.h"
#include "Application.h"
#include "Globals.h"


#define VERTEX_ATTRIBUTES 3

struct Mesh {
    uint id_vertex = 0;
    uint vertexCount = 0;
    float* vertex = nullptr;

    uint id_index = 0;
    uint indexCount = 0;
    uint* index = nullptr;

    uint VBO = 0;//Buffer Vertex data
    uint EBO = 0;//Buffer Index data

    glm::vec3 position;

    void Render();
};

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

    void LoadMesh(const char* file_path);
    void ImportMesh(aiMesh* aiMesh);
    void RenderScene();

private:
    std::vector<Mesh*> meshList;
    const char* file_path;
};

#endif