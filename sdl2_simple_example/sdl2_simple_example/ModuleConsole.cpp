#include "ModuleConsole.h"
#include "Application.h"
#include "Globals.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

ModuleConsole::ModuleConsole(Application* app, bool start_enabled)
    : Module(app, start_enabled), autoScroll(true), showConsole(true), consoleWindow(nullptr), consoleContext(nullptr) {}

ModuleConsole::~ModuleConsole() {
    CleanUp();
}

bool ModuleConsole::Init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize: %s", SDL_GetError());
        return false;
    }

    consoleWindow = SDL_CreateWindow("Console", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (consoleWindow == nullptr) {
        SDL_Log("Could not create console window: %s", SDL_GetError());
        return false;
    }

    consoleContext = SDL_GL_CreateContext(consoleWindow);
    if (consoleContext == nullptr) {
        SDL_Log("Could not create OpenGL context: %s", SDL_GetError());
        return false;
    }

    SDL_GL_MakeCurrent(consoleWindow, consoleContext);
    SDL_GL_SetSwapInterval(1);  

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(consoleWindow, consoleContext);
    ImGui_ImplOpenGL3_Init("#version 130");  

    AddLog("Console initialized.");
    return true;
}

bool ModuleConsole::Start() {
    return true;
}

update_status ModuleConsole::PreUpdate(float dt) {
    ImGui_ImplSDL2_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
    return UPDATE_CONTINUE;
}

update_status ModuleConsole::Update(float dt) {
    if (showConsole) {
        ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
        ImGui::Begin("Engine Console", &showConsole);

        if (ImGui::Button("Clear")) {
            logs.clear();
        }
        ImGui::SameLine();
        ImGui::Checkbox("Auto-scroll", &autoScroll);

        ImGui::BeginChild("LogScrollRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        for (const auto& log : logs) {
            ImGui::TextUnformatted(log.c_str());
        }

        if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::EndChild();
        ImGui::End();
    }
    return UPDATE_CONTINUE;
}

update_status ModuleConsole::PostUpdate(float dt) {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(consoleWindow);

    return UPDATE_CONTINUE;
}

bool ModuleConsole::CleanUp() {
    logs.clear();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    if (consoleContext) {
        SDL_GL_DeleteContext(consoleContext);
        consoleContext = nullptr;
    }

    if (consoleWindow) {
        SDL_DestroyWindow(consoleWindow);
        consoleWindow = nullptr;
    }

    SDL_Quit();
    return true;
}

void ModuleConsole::AddLog(const char* log) {
    logs.push_back(log);
}