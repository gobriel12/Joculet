#include "GlobalPath.h"


C9Engine::GlobalPath* C9Engine::GlobalPath::ms_Instance = nullptr;
const char* C9Engine::GlobalPath::ms_MainWindowTitle = "Dear ImGui SDL2+SDL_Renderer example";
const int C9Engine::GlobalPath::ms_Weight = 1280;
const int C9Engine::GlobalPath::ms_Height = 980;

C9Engine::GlobalPath::GlobalPath(int, char** argv)
	: m_EXE_PATH(argv[0])
{
}

C9Engine::GlobalPath* C9Engine::GlobalPath::Instance(int argc, char** argv)
{
	if (ms_Instance != nullptr)
	{
		return ms_Instance;
	}
	if (argc == 0 || argv == nullptr)
	{
		return nullptr;
	}
	return ms_Instance = new GlobalPath(argc, argv);
}
