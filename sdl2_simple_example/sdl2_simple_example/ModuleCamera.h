#ifndef __ModuleCamera_H__
#define __ModuleCamera_H__

#include "Module.h"
#include "SDL2/SDL.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ModuleWindow.h"

class Application;

class ModuleCamera : public Module {
public:
    ModuleCamera(Application* app, bool start_enabled = true);
    ~ModuleCamera();

    bool Init() override;
    bool Start() override;
    update_status PreUpdate(float dt) override;
    update_status Update(float dt) override;
    update_status PostUpdate(float dt) override;
    bool CleanUp() override;

    void CameraInput(float dt);
    void updateViewMatrix();

    const glm::mat4& GetViewMatrix() const { return viewMatrix; }

private:
    float speed; 

    glm::vec3 position;        
    glm::vec3 target;          
    glm::vec3 up;
    glm::vec3 front;
    glm::mat4 viewMatrix;      
    glm::mat4 projectionMatrix;
};
#endif