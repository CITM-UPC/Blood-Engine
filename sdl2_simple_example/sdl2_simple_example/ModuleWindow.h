#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL2/SDL.h"

class Application;
bool processEvents();


class ModuleWindow : public Module {
public:


    ModuleWindow(Application* app, bool start_enabled = true);
    ~ModuleWindow();

    bool Init() override;
    bool Start() override;
    update_status PreUpdate(float dt) override;
    update_status Update(float dt) override;
    update_status PostUpdate(float dt) override;
    bool CleanUp() override;

    void SetTitle(const char* title);
    void SwapBuffers();

public:
    SDL_Window* window;
    SDL_Surface* screenSurface;
    SDL_GLContext context;
    int Height;
    int Width;
};
#endif