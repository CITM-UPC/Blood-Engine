#include "ModuleCamera.h"
#include "Application.h"
#include "Globals.h"
#include <glm/gtc/matrix_transform.hpp>
#include "ModuleWindow.h"

ModuleCamera::ModuleCamera(Application* app, bool start_enabled) : Module(app, start_enabled) {
    position = glm::vec3(10.0f, 10.0f, 10.0f);
    front = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));  
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    speed = 20.0f;
}

ModuleCamera::~ModuleCamera() {
    CleanUp();
}

bool ModuleCamera::Init() {
    return true;
}

bool ModuleCamera::Start() {
    return true;
}

update_status ModuleCamera::PreUpdate(float dt) {
    return UPDATE_CONTINUE;
}

update_status ModuleCamera::Update(float dt) {
    CameraInput(dt);
    updateViewMatrix();
    glLoadIdentity();

    gluPerspective(45.0f, (GLfloat)App->window->Width / (GLfloat)App->window->Height, 0.1f, 100.0f);

    gluLookAt(position.x, position.y, position.z,
        position.x + front.x, position.y + front.y, position.z + front.z,
        up.x, up.y, up.z);

    return UPDATE_CONTINUE;
}

update_status ModuleCamera::PostUpdate(float dt) {
    return UPDATE_CONTINUE;
}

void ModuleCamera::CameraInput(float dt) {
    const Uint8* keys = SDL_GetKeyboardState(NULL);

    // Movimiento hacia adelante y atrás
    if (keys[SDL_SCANCODE_W]) {
        position += front * speed * dt;
        SDL_Log("pulsas w");
    }
    if (keys[SDL_SCANCODE_S]) {
        position -= front * speed * dt;
        SDL_Log("pulsas s");
    }

    // Movimiento hacia los lados
    glm::vec3 right = glm::normalize(glm::cross(front, up));
    if (keys[SDL_SCANCODE_A]) {
        position -= right * speed * dt;
        SDL_Log("pulsas a");
    }
    if (keys[SDL_SCANCODE_D]) {
        position += right * speed * dt;
        SDL_Log("pulsas d");
    }

    int mouseMotionX, mouseMotionY;
    SDL_GetRelativeMouseState(&mouseMotionX, &mouseMotionY);

    float sensitivity = 0.1f;
    float yaw = mouseMotionX * sensitivity;
    float pitch = mouseMotionY * sensitivity;

    glm::mat4 yawRotation = glm::rotate(glm::mat4(1.0f), glm::radians(yaw), up);
    front = glm::normalize(glm::vec3(yawRotation * glm::vec4(front, 1.0f)));

    glm::vec3 horizontalRight = glm::normalize(glm::cross(up, front));
    glm::mat4 pitchRotation = glm::rotate(glm::mat4(1.0f), glm::radians(pitch), horizontalRight);
    front = glm::normalize(glm::vec3(pitchRotation * glm::vec4(front, 1.0f)));
}

void ModuleCamera::updateViewMatrix() {
    viewMatrix = glm::lookAt(position, position + front, up);  
}

bool ModuleCamera::CleanUp() {
    return true;
}