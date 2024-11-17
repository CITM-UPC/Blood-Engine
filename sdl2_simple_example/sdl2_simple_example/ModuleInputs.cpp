#include "Application.h"
#include "ModuleInputs.h"
#include "imgui_impl_sdl2.h"
#include "Globals.h"
#include "ModuleWindow.h"
#include "ModuleRenderer.h"


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



bool ModuleInputs::Init()
{
    SDL_Init(0);
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        std::cerr << "Error al inicializar SDL: " << SDL_GetError() << std::endl;
        return false;
    }
    SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
    SDL_EventState(SDL_WINDOWEVENT, SDL_ENABLE);
    return true;
}
bool ModuleInputs::Start()
{
    return true;
}
update_status ModuleInputs::PreUpdate(float dt)
{
    //SDL_PumpEvents();

    mousexmotion = 0;
    mouseymotion = 0;

    SDL_Event event;
    int eventCount = 0;
    //DEbug
    std::this_thread::sleep_for(std::chrono::milliseconds(1));//ESTO HACE QUE CAPTURE EL EVENTO DE DRAG AND DROP PROVISIONAL
                                                              //Hay que arreglar la interferencias del ProcessEvents de ModuleWindow con el de aqui
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event); 
        eventCount++;
        std::string extension;
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

            //Drag & Drop
        case SDL_DROPFILE:
            dropped_file_path = event.drop.file;
            dropped_file_path_str = event.drop.file;

            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "File dropped", dropped_file_path, App->window->window);
            extension = dropped_file_path_str.substr(dropped_file_path_str.find_last_of(".") + 1);
            if (extension == "FBX" || extension == "fbx")
            {
                std::cout << "Loading FBX" << std::endl;
                App->renderer->LoadMesh(dropped_file_path, true);
            }
            SDL_free(dropped_file_path);
            break;
        }
    }
    //std::cerr << "Eventos procesados en este frame: " << eventCount << std::endl;

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