#include <string>
#include <SDL.h>

bool g_Running = false;

// Window
SDL_Window* g_pWindow = nullptr;
int g_WindowID = 0;
int g_Width = 0;
int g_Height = 0;

//Renderer
SDL_Renderer* g_pRenderer = nullptr;
SDL_Color g_BackgroundColor = SDL_Color{ 0x00, 0x00, 0x00, 0xFF };

void WindowRelease()
{
	// Destroy the window
	SDL_DestroyWindow(g_pWindow);
	g_pWindow = nullptr;
}

void RendererRelease()
{
	SDL_DestroyRenderer(g_pRenderer);
	g_pRenderer = nullptr;
}

void RendererClearBackBuffer()
{
	SDL_SetRenderDrawColor(g_pRenderer, g_BackgroundColor.r, g_BackgroundColor.g, g_BackgroundColor.b, g_BackgroundColor.a);
	SDL_RenderClear(g_pRenderer);
}

bool CreateRenderer(int index, Uint32 flags)
{
	if (!g_pWindow)
		return false;

	RendererRelease();

	g_pRenderer = SDL_CreateRenderer(g_pWindow, index, flags);
	if (g_pRenderer == nullptr)
		return false;

	RendererClearBackBuffer();

	return true;
}

bool WindowCreate(std::string title, int x, int y, int w, int h, Uint32 flags)
{
	WindowRelease();

	g_pWindow = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);

	if (g_pWindow == nullptr)
		return false;

	g_Width = w;
	g_Height = h;

	//Grab window identifier 
	g_WindowID = SDL_GetWindowID(g_pWindow);

	return true;
}

void Err2MsgBox(std::string err_msg)
{
	err_msg += SDL_GetError();
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", err_msg.c_str(), nullptr);
}

bool Init()
{
	// Initialise SDL, report error if it fails
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		Err2MsgBox("SDL Initialisation Failed!\n");
		return false;
	}

	// Create a window, report error if window not created
	if (!WindowCreate("Test Window",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640, 480,
		SDL_WINDOW_SHOWN))
	{
		Err2MsgBox("Window Creation Failed.\n");
		return false;
	}

	// Create a renderer, report error if window not created
	if (!CreateRenderer(-1, SDL_RENDERER_ACCELERATED))
	{
		Err2MsgBox("Renderer Creation Failed.\n");
		return false;
	}

	return true;
}

void Cleanup()
{
	RendererRelease();
	WindowRelease();

	// Shutdown SDL
	SDL_Quit();
}

void HandleEvents()
{
	SDL_Event Event;

	while (SDL_PollEvent(&Event))
	{
		if (Event.type == SDL_QUIT)
			g_Running = false;
	}
}

void Render()
{
	SDL_RenderPresent(g_pRenderer);

	// Clear the backbuffer ready for next frame
	RendererClearBackBuffer();
}

void MainLoop()
{
	g_Running = true;

	while (g_Running)
	{
		HandleEvents();

		Render();
	}
}

int main(int argc, char *argv[])
{
	// Initialise SDL and create window
	if (!Init())
		return -1;

	MainLoop();

	Cleanup();

	return 0;
}