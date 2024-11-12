#include "Application.h"
#include "ModuleInputs.h"
#include "imgui_impl_sdl2.h"

ModuleInputs::ModuleInputs(Application* app, bool start_enabled)
    : Module(app, start_enabled), mousex(0), mousey(0), mousez(0), mousexmotion(0), mouseymotion(0)
{
    keyboard = new KEY_STATE[MAX_KEYS];
    memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
    memset(mbutton, KEY_IDLE, sizeof(KEY_STATE) * MAX_BUTTONS);
}

ModuleInputs::~ModuleInputs()
{
    delete[] keyboard;
}

bool ModuleInputs::Start()
{
    return true;
}

bool ModuleInputs::Init()
{
    SDL_InitSubSystem(SDL_INIT_EVENTS); 
    return true;
}

update_status ModuleInputs::PreUpdate(float dt)
{
    mousexmotion = 0;
    mouseymotion = 0;

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event); 

        switch (event.type)
        {
        case SDL_QUIT:
            return UPDATE_STOP;

        case SDL_KEYDOWN:
        case SDL_KEYUP:
        {
            int key = event.key.keysym.scancode;
            if (event.type == SDL_KEYDOWN)
            {
                keyboard[key] = (keyboard[key] == KEY_IDLE) ? KEY_DOWN : KEY_REPEAT;
            }
            else
            {
                keyboard[key] = KEY_UP;
            }
            break;
        }

        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        {
            int button = event.button.button - 1;
            if (button < MAX_BUTTONS)
            {
                mbutton[button] = (event.type == SDL_MOUSEBUTTONDOWN) ? KEY_DOWN : KEY_UP;
            }
            break;
        }

        case SDL_MOUSEMOTION:
            mousex = event.motion.x;
            mousey = event.motion.y;
            mousexmotion = event.motion.xrel;
            mouseymotion = event.motion.yrel;
            break;

        case SDL_MOUSEWHEEL:
            mousez = event.wheel.y;
            break;
        }
    }

    // estados de teclado
    for (int i = 0; i < MAX_KEYS; ++i)
    {
        if (keyboard[i] == KEY_DOWN)
            keyboard[i] = KEY_REPEAT;
        else if (keyboard[i] == KEY_UP)
            keyboard[i] = KEY_IDLE;
    }

    // estados botones del rat�n
    for (int i = 0; i < MAX_BUTTONS; ++i)
    {
        if (mbutton[i] == KEY_DOWN)
            mbutton[i] = KEY_REPEAT;
        else if (mbutton[i] == KEY_UP)
            mbutton[i] = KEY_IDLE;
    }

    return UPDATE_CONTINUE;
}

update_status ModuleInputs::Update(float dt)
{
    return UPDATE_CONTINUE;
}

update_status ModuleInputs::PostUpdate(float dt)
{
    return UPDATE_CONTINUE;
}

bool ModuleInputs::CleanUp()
{
    SDL_QuitSubSystem(SDL_INIT_EVENTS); 
    return true;
}