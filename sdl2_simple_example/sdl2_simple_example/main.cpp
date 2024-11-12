#include "Defs.h"
#include "Application.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

using namespace std;

using hrclock = chrono::high_resolution_clock;
using u8vec4 = glm::u8vec4;
using ivec2 = glm::ivec2;
using vec3 = glm::dvec3;

static const ivec2 WINDOW_SIZE(1024, 1024);
static const unsigned int FPS = 60;
static const auto FRAME_DT = 1.0s / FPS;

static void init_openGL() {
	glewInit();
	if (!GLEW_VERSION_3_0) throw exception("OpenGL 3.0 API is not available.");
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.5, 0.5, 0.5, 1.0);
}

static void init_deviL() {
	ilInit();
}



enum main_states
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

int main(int argc, char** argv) 
{
	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;
	Application* App = NULL;

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:

			App = new Application();
			state = MAIN_START;
			break;

		case MAIN_START:
			if (App->Init() == false)
			{
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
			}

			break;

		case MAIN_UPDATE:
		{

			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				state = MAIN_EXIT;
			}
			else if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;

			break;

		}

		case MAIN_FINISH:

			if (App->CleanUp() == false)
			{ }
			
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}
	}

	External = nullptr;
	delete App;

	return main_return;
}