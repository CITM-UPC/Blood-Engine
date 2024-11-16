#include "ModuleWindow.h"
#include "ModuleRenderer.h"

#include "Application.h"
#include "Globals.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "ModuleCamera.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
    window = nullptr;
    screenSurface = nullptr;
    context = nullptr;
}

ModuleWindow::~ModuleWindow()
{
    CleanUp();
}

bool ModuleWindow::Init()
{
    bool ret = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        ret = false;
    }
    else
    {
        int width = SCREEN_WIDTH * SCREEN_SIZE;
        int height = SCREEN_HEIGHT * SCREEN_SIZE;
        Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

        if (WIN_FULLSCREEN)
            flags |= SDL_WINDOW_FULLSCREEN;
        if (WIN_RESIZABLE)
            flags |= SDL_WINDOW_RESIZABLE;
        if (WIN_BORDERLESS)
            flags |= SDL_WINDOW_BORDERLESS;
        if (WIN_FULLSCREEN_DESKTOP)
            flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

        window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
        Width = width;
        Height = height;

        if (window == nullptr)
        {
            ret = false;
        }
        else
        {
            context = SDL_GL_CreateContext(window);

            ImGui::CreateContext();
            ImGui_ImplSDL2_InitForOpenGL(window, context);
            ImGui_ImplOpenGL3_Init("#version 130");

            if (context == nullptr)
            {
                SDL_Log("No se pudo crear el contexto OpenGL: %s", SDL_GetError());
                ret = false;
            }
            else
            {
                SDL_GL_MakeCurrent(window, context);
                SDL_GL_SetSwapInterval(1);
                glEnable(GL_DEPTH_TEST);
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            }
        }
    }
    return ret;
}

bool processEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            return false;
            break;
        default:
            ImGui_ImplSDL2_ProcessEvent(&event);
            break;
        }
    }
    return true;
}

bool ModuleWindow::CleanUp()
{
    if (context != nullptr)
    {
        SDL_GL_DeleteContext(context);
    }

    if (window != nullptr)
    {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
    return true;
}

void ModuleWindow::SetTitle(const char* title)
{
    if (window != nullptr)
    {
        SDL_SetWindowTitle(window, title);
    }
}

void ModuleWindow::SwapBuffers()
{
    SDL_GL_SwapWindow(window);

}

bool ModuleWindow::Start() {
    return true;
}

update_status ModuleWindow::PreUpdate(float dt) {
    return UPDATE_CONTINUE; 
}

const int GRID_SIZE = 20;  
const float LINE_SPACING = 1.0f;

void DrawGrid() {
    glBegin(GL_LINES);


    glColor3f(1.0f, 0.0f, 0.0f); 
    glVertex3f(-GRID_SIZE * LINE_SPACING, 0.0f, 0.0f);
    glVertex3f(GRID_SIZE * LINE_SPACING, 0.0f, 0.0f); 


    glColor3f(0.0f, 0.0f, 1.0f);  
    glVertex3f(0.0f, 0.0f, -GRID_SIZE * LINE_SPACING); 
    glVertex3f(0.0f, 0.0f, GRID_SIZE * LINE_SPACING);  

    glColor3f(0.0f, 1.0f, 0.0f);  
    glVertex3f(0.0f, -GRID_SIZE * LINE_SPACING, 0.0f); 
    glVertex3f(0.0f, GRID_SIZE * LINE_SPACING, 0.0f);  

    glColor3f(0.5f, 0.5f, 0.5f); 
    for (int i = -GRID_SIZE; i <= GRID_SIZE; ++i) {
        
        glVertex3f(i * LINE_SPACING, 0.0f, -GRID_SIZE * LINE_SPACING);
        glVertex3f(i * LINE_SPACING, 0.0f, GRID_SIZE * LINE_SPACING);

        glVertex3f(-GRID_SIZE * LINE_SPACING, 0.0f, i * LINE_SPACING);
        glVertex3f(GRID_SIZE * LINE_SPACING, 0.0f, i * LINE_SPACING);
    }

    glEnd();
}

update_status ModuleWindow::Update(float dt)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    if (ImGui::BeginMainMenuBar()) {

        if (ImGui::BeginMenu("Menu")) {
            if (ImGui::MenuItem("Adeu")) {
                SDL_Event quit_event;
                quit_event.type = SDL_QUIT;
                SDL_PushEvent(&quit_event);
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Objects")) 
        {

            if (ImGui::MenuItem("Create Plane")) 
            {

            }

            if (ImGui::MenuItem("Create Cube")) 
            {

            }

            if (ImGui::MenuItem("Create Sphere")) 
            {

            }

            if (ImGui::MenuItem("Create Pyramid")) 
            {

            }

            ImGui::EndMenu(); 
        }

        ImGui::EndMainMenuBar();
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    DrawGrid();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SwapBuffers();

    return UPDATE_CONTINUE;
}

update_status ModuleWindow::PostUpdate(float dt) {
    return UPDATE_CONTINUE; 
}
