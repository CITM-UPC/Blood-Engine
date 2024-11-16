#include "Application.h"

#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInputs.h"
//#include "ModuleConsole.h"
#include "ModuleCamera.h"
#include "ModuleRenderer.h"



extern Application* External = nullptr;

Application::Application()
{
	External = this;

	window = new ModuleWindow(this);
	inputs = new ModuleInputs(this);
	camera = new ModuleCamera(this);
	renderer = new ModuleRenderer(this);
	//console = new ModuleConsole(this);
	AddModule(window);
	AddModule(inputs);
	AddModule(camera);
	AddModule(renderer);
	//AddModule(console);
}

Application::~Application()
{
	for (std::vector<Module*>::iterator it = list_modules.begin(); it != list_modules.end(); ++it)
	{
		delete (*it);
		(*it) = nullptr;
	}
}

bool Application::Init()
{
	bool ret = true;

	for (std::vector<Module*>::const_iterator it = list_modules.cbegin(); it != list_modules.cend() && ret; ++it)
	{
		(*it)->Init();
	}

	for (std::vector<Module*>::const_iterator it = list_modules.cbegin(); it != list_modules.cend() && ret; ++it)
	{
		(*it)->Start();
	}

	ms_timer.Start();
	return ret;
}

void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

void Application::FinishUpdate()
{
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	if (!processEvents()) ret = UPDATE_STOP;

	PrepareUpdate();

	for (std::vector<Module*>::const_iterator it = list_modules.cbegin(); it != list_modules.cend() && ret == UPDATE_CONTINUE; ++it)
	{
		(*it)->PreUpdate(dt);
	}

	for (std::vector<Module*>::const_iterator it = list_modules.cbegin(); it != list_modules.cend() && ret == UPDATE_CONTINUE; ++it)
	{
		(*it)->Update(dt);
	}

	for (std::vector<Module*>::const_iterator it = list_modules.cbegin(); it != list_modules.cend() && ret == UPDATE_CONTINUE; ++it)
	{
		(*it)->PostUpdate(dt);
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	for (std::vector<Module*>::reverse_iterator it = list_modules.rbegin(); it != list_modules.rend() && ret; ++it)
	{
		ret = (*it)->CleanUp();
	}
	return ret;
}

float Application::GetFPS()
{
	return 1 / dt;
}

float Application::GetDT()
{
	return dt;
}

float Application::GetMS()
{
	return dt * 1000;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}