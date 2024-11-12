#pragma once
#include "Module.h"
#include "Globals.h"
#include <SDL2/SDL.h>

#define MAX_KEYS 200  
#define MAX_BUTTONS 5

bool processevents();
enum KEY_STATE {
    KEY_IDLE,
    KEY_DOWN,
    KEY_REPEAT,
    KEY_UP
};

class ModuleInputs : public Module
{
public:
    ModuleInputs(Application* app, bool start_enabled = true);
    ~ModuleInputs();

    bool Init() override;
    bool Start() override;
    update_status PreUpdate(float dt) override;
    update_status Update(float dt) override;
    update_status PostUpdate(float dt) override;
    bool CleanUp() override;

    KEY_STATE GetKey(int id) const { return keyboard[id]; }
    KEY_STATE GetMButton(int id) const { return mbutton[id]; }
    int GetMouseX() const { return mousex; }
    int GetMouseY() const { return mousey; }
    int GetMouseZ() const { return mousez; }
    int GetMouseXMotion() const { return mousexmotion; }
    int GetMouseYMotion() const { return mouseymotion; }

private:
    KEY_STATE* keyboard;
    KEY_STATE mbutton[MAX_BUTTONS];
    int mousex, mousey, mousez;
    int mousexmotion, mouseymotion;
};