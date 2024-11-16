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

// Definición del número de atributos de vértices
#define VERTEX_ATTRIBUTES 3

ModuleRenderer::ModuleRenderer(Application* app, bool start_enabled) : Module(app, start_enabled) {}

ModuleRenderer::~ModuleRenderer() {
    CleanUp();
}

bool ModuleRenderer::Init() {
    // Inicialización de OpenGL, DevIL, etc.
    // En algún lugar de tu código (por ejemplo, en Init() o Start())
    glewInit();
    if (!GLEW_VERSION_3_0) throw exception("OpenGL 3.0 API is not available.");
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5, 0.5, 0.5, 1.0);
    ilInit();

    //if (glewInit() != GLEW_OK) {
    //    std::cerr << "Error al inicializar GLEW!" << std::endl;
    //    return false; // O algún manejo de error adecuado
    //}
    //return true;

}

bool ModuleRenderer::Start() {
    // Aquí puedes cargar tu malla desde el archivo
    std::cerr << "Inicio de Start()" << std::endl;  // Diagnóstico adicional

    const char* rutaRelativa = "Assets/BakerHouse.fbx"; // tu ruta relativa
    char rutaAbsoluta[_MAX_PATH];
    if (_fullpath(rutaAbsoluta, rutaRelativa, _MAX_PATH) != nullptr) {
        std::cout << "Ruta absoluta: " << rutaAbsoluta << std::endl;
    }
    else {
        std::cerr << "Error al convertir la ruta." << std::endl;
    }


    LoadMesh("Assets/BakerHouse.fbx");  // Llama a LoadMesh al inicio
    //Debug PATH (PQ NO CARGA?)


    return true;
}

update_status ModuleRenderer::PreUpdate(float dt) {
    return UPDATE_CONTINUE;
}

update_status ModuleRenderer::Update(float dt) {
    // Renderizamos la escena en cada actualización
    return UPDATE_CONTINUE;
}

update_status ModuleRenderer::PostUpdate(float dt) {
    RenderScene();
    return UPDATE_CONTINUE;
}

bool ModuleRenderer::CleanUp() {
    // Limpiamos todos los recursos utilizados (mallas, buffers, etc.)
    for (Mesh* mesh : meshList) {
        delete[] mesh->vertex;
        delete[] mesh->index;
        delete mesh;
    }
    meshList.clear();
    return true;
}

void ModuleRenderer::RenderScene() {
    // Renderizamos todas las mallas en la lista
    for (Mesh* mesh : meshList) {
        mesh->Render();
    }
}

void ModuleRenderer::LoadMesh(const char* file_path) {

    const aiScene* currentScene = aiImportFile(file_path, aiProcess_Triangulate);

    // Si la escena tiene meshList
    if (currentScene != nullptr && currentScene->HasMeshes())
    {
        std::cerr << "Escena tiene mesh" << std::endl;

        for (int i = 0; i < currentScene->mNumMeshes; i++)
        {
            ImportMesh(currentScene->mMeshes[i]);
        }
        aiReleaseImport(currentScene);
        std::cerr << "Hola" << std::endl;

    }
    else
    {
        std::cerr << "Error loading mesh from file " << file_path << std::endl;
    }
}

void  ModuleRenderer::ImportMesh(aiMesh* aiMesh)
{
    Mesh* currentMesh = new Mesh();
    //DEBUG
    if (currentMesh == nullptr) {
        std::cerr << "Error: No se pudo asignar memoria para currentMesh." << std::endl;
        return;
    }
    //DEBUG
    //Guardando numero de vertices desde malla ASSIMP
    currentMesh->vertexCount = aiMesh->mNumVertices;

    //Reservamos memoria para vertices ( 3 componentes , de momento)
    currentMesh->vertex = new float[currentMesh->vertexCount * 3];
    //DEBUG
    if (currentMesh->vertex == nullptr) {
        std::cerr << "Error: No se pudo asignar memoria para los vértices." << std::endl;
        delete currentMesh;
        return;
    }
    //DEBUG
    memcpy(currentMesh->vertex, aiMesh->mVertices, sizeof(float) * currentMesh->vertexCount * 3);

    // Si tiene caras, cargamos caras
    if (aiMesh->HasFaces())
    {
        currentMesh->indexCount = aiMesh->mNumFaces * 3;
        currentMesh->index = new uint[currentMesh->indexCount];
        //DEBUG
        if (currentMesh->index == nullptr) {
            std::cerr << "Error: No se pudo asignar memoria para los índices." << std::endl;
            delete[] currentMesh->vertex;
            delete currentMesh;
            return;
        }
        //DEBUG
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

        if (currentMesh->VBO == 0 || currentMesh->EBO == 0) {
            std::cerr << "Error al generar los buffers de OpenGL." << std::endl;
            delete[] currentMesh->vertex;
            delete[] currentMesh->index;
            delete currentMesh;
            return;
        }

        //Bindeando y rellenando Buffers
        glBindBuffer(GL_ARRAY_BUFFER, currentMesh->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * currentMesh->vertexCount * VERTEX_ATTRIBUTES, currentMesh->vertex, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentMesh->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * currentMesh->indexCount, currentMesh->index, GL_STATIC_DRAW);

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

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glVertexPointer(3, GL_FLOAT, 0, NULL);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, NULL);

    glDisableClientState(GL_VERTEX_ARRAY);
}
