#include <string>

#include "framework.h"
#include "Main.h"

#include "Editor.h"
#include "Render/RenderEngine.h"


int APIENTRY WinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{

	Editor* pEditor = new Editor();
    pEditor->Run();

    return 0;
}