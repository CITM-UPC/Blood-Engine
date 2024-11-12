#ifndef MODULECONSOLE_H
#define MODULECONSOLE_H

#include "Module.h"
#include <vector>
#include <string>
#include "SDL2/SDL.h"

class ModuleConsole : public Module {
public:
    ModuleConsole(Application* app, bool start_enabled = true);
    ~ModuleConsole();

    bool Init() override;
    bool Start() override;
    update_status PreUpdate(float dt) override;
    update_status Update(float dt) override;
    update_status PostUpdate(float dt) override;
    bool CleanUp() override;

    void AddLog(const char* log);

private:
    std::vector<std::string> logs;
    bool autoScroll;
    bool showConsole;

    SDL_Window* consoleWindow;
    SDL_GLContext consoleContext;
};

#endif // MODULECONSOLE_H