#include "ModuleRenderer.h"
#include "Application.h"
#include "Defs.h"
#include "ModuleWindow.h"

#include "Globals.h"
#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


ModuleRenderer::ModuleRenderer(Application* app, bool start_enabled) : Module(app, start_enabled) {}

ModuleRenderer::~ModuleRenderer() {
    CleanUp();
}

bool ModuleRenderer::Init() {

    return true;

}

bool ModuleRenderer::Start() {
    //Debug PATH (PQ NO CARGA?)
    /*
    const char* rutaRelativa = "Assets/BakerHouse.fbx"; // tu ruta relativa
    char rutaAbsoluta[_MAX_PATH];
    if (_fullpath(rutaAbsoluta, rutaRelativa, _MAX_PATH) != nullptr) {
        std::cout << "Ruta absoluta: " << rutaAbsoluta << std::endl;
    }
    else {
        std::cerr << "Error al convertir la ruta." << std::endl;
    }
    */
    LoadMesh("Assets/BakerHouse.fbx",false);

    return true;
}

update_status ModuleRenderer::PreUpdate(float dt) {
    return UPDATE_CONTINUE;
}

update_status ModuleRenderer::Update(float dt) {
    return UPDATE_CONTINUE;
}

update_status ModuleRenderer::PostUpdate(float dt) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    RenderScene();
    return UPDATE_CONTINUE;
}

bool ModuleRenderer::CleanUp() {
    for (Mesh* mesh : meshList) {
        delete[] mesh->vertex;
        delete[] mesh->index;
        delete mesh;
    }
    meshList.clear();
    return true;
}

void ModuleRenderer::RenderScene() {
    for (int i = 0; i < meshList.size(); i++) {
        glPushMatrix();

        // Aplicar traslación
        glTranslatef(meshList[i]->position.x, 0.0f, meshList[i]->position.z);

        meshList[i]->Render();
        glPopMatrix();
    }
}

void ModuleRenderer::LoadMesh(const char* file_path, bool apply_random_translation) {

    const aiScene* currentScene = aiImportFile(file_path, aiProcess_Triangulate);

    // Si la escena tiene meshList
    if (currentScene != nullptr && currentScene->HasMeshes())
    {
        std::cerr << "Escena tiene mesh" << std::endl;

        for (int i = 0; i < currentScene->mNumMeshes; i++)
        {
            ImportMesh(currentScene->mMeshes[i],apply_random_translation);
        }
        aiReleaseImport(currentScene);
    }
    else
    {
        std::cerr << "Error loading mesh from file " << file_path << std::endl;
    }
}

void  ModuleRenderer::ImportMesh(aiMesh* aiMesh, bool apply_random_translation)
{
    Mesh* currentMesh = new Mesh();

    //Guardando numero de vertices desde malla ASSIMP
    currentMesh->vertexCount = aiMesh->mNumVertices;

    //Reservamos memoria para vertices ( 3 componentes , de momento)
    currentMesh->vertex = new float[currentMesh->vertexCount * 3];
    memcpy(currentMesh->vertex, aiMesh->mVertices, sizeof(float) * currentMesh->vertexCount * 3);

    // Si tiene caras, cargamos caras
    if (aiMesh->HasFaces())
    {
        currentMesh->indexCount = aiMesh->mNumFaces * 3;
        currentMesh->index = new uint[currentMesh->indexCount];

        for (uint i = 0; i < aiMesh->mNumFaces; ++i)
        {
            if (aiMesh->mFaces[i].mNumIndices == 3)
            {
                memcpy(&currentMesh->index[i * 3], aiMesh->mFaces[i].mIndices, 3 * sizeof(uint));

            }
            else
            {
                std::cerr << "WARNING!!, face with more or less than 3 indices!" << std::endl;
            }
        }

        currentMesh->VBO = 0;//Vertices Buffer
        currentMesh->EBO = 0;//Indices Buffer

        //Generando Buffers de vértices e índices
        glGenBuffers(1, &currentMesh->VBO);
        glGenBuffers(1, &currentMesh->EBO);

        //Bindeando y rellenando Buffers
        glBindBuffer(GL_ARRAY_BUFFER, currentMesh->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * currentMesh->vertexCount * VERTEX_ATTRIBUTES, currentMesh->vertex, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentMesh->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * currentMesh->indexCount, currentMesh->index, GL_STATIC_DRAW);

        //Creado Unicamente para mostrar el Drag&Drop
        if (apply_random_translation) {
            currentMesh->position = glm::vec3(static_cast<float>(rand() % 10 - 5),0.0f,static_cast<float>(rand() % 10 - 5)
            );
        }
        else {
            currentMesh->position = glm::vec3(0.0f, 0.0f, 0.0f);
        }

        //Mesh añadida al vector que contiene las meshes
        meshList.push_back(currentMesh);
    }
    else
    {
        delete currentMesh;
    }
}

void Mesh::Render() {
    if (VBO == 0 || EBO == 0) {
        std::cerr << "Error: VBO or EBO not initialized!" << std::endl;
        return;
    }

    glEnableClientState(GL_VERTEX_ARRAY);

    //Vinculamos Buffers otra vez
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexPointer(3, GL_FLOAT, 0, NULL);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    //Dibujar
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, NULL);

    glDisableClientState(GL_VERTEX_ARRAY);
}
