#include "ModuleRenderer.h"
#include "Application.h"
#include "Globals.h"
#include <GL/glew.h>
#include <iostream>

ModuleRenderer::ModuleRenderer(Application* app, bool start_enabled) : Module(app, start_enabled) {
}

ModuleRenderer::~ModuleRenderer() {
    CleanUp();
}

bool ModuleRenderer::Init() {
    return true;
}

bool ModuleRenderer::Start() {
    return true;
}

update_status ModuleRenderer::PreUpdate(float dt) {
    return UPDATE_CONTINUE;
}

update_status ModuleRenderer::Update(float dt) {
    for (auto& mesh : meshes) {
        mesh.Draw();
    }
    return UPDATE_CONTINUE;
}

update_status ModuleRenderer::PostUpdate(float dt) {
    return UPDATE_CONTINUE;
}

bool ModuleRenderer::LoadModel(const char* path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate |
        aiProcess_GenNormals |
        aiProcess_CalcTangentSpace |
        aiProcess_JoinIdenticalVertices
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return false;
    }

    directory = std::string(path).substr(0, std::string(path).find_last_of('/'));
    ProcessNode(scene->mRootNode, scene);

    return true;
}

void ModuleRenderer::ProcessNode(aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene));
    }

    // Procesar nodos hijos
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);
    }
}

ModuleRenderer::Mesh ModuleRenderer::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    Mesh newMesh;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        vertex.Position.x = mesh->mVertices[i].x;
        vertex.Position.y = mesh->mVertices[i].y;
        vertex.Position.z = mesh->mVertices[i].z;

        if (mesh->HasNormals()) {
            vertex.Normal.x = mesh->mNormals[i].x;
            vertex.Normal.y = mesh->mNormals[i].y;
            vertex.Normal.z = mesh->mNormals[i].z;
        }

        if (mesh->mTextureCoords[0]) {
            vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
            vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
        }
        else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        newMesh.vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            newMesh.indices.push_back(face.mIndices[j]);
    }

    newMesh.SetupMesh();
    return newMesh;
}

void ModuleRenderer::Mesh::SetupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}

void ModuleRenderer::Mesh::Draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

bool ModuleRenderer::CleanUp() {
    for (auto& mesh : meshes) {
        glDeleteVertexArrays(1, &mesh.VAO);
        glDeleteBuffers(1, &mesh.VBO);
        glDeleteBuffers(1, &mesh.EBO);
    }
    meshes.clear();
    return true;
}