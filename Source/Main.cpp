#include <string>
#include <SDL.h>
#include <SDL_image.h>

#include "SDLErrorReport.h"
#include "Renderer.h"
#include "Texture.h"

bool g_Running = false;

// Window
SDL_Window* g_pWindow = nullptr;
int g_WindowID = 0;
int g_Width = 0;
int g_Height = 0;

//Renderer
Renderer g_Renderer;

//Texture
Texture g_Texture;

void WindowRelease()
{
	// Destroy the window
	SDL_DestroyWindow(g_pWindow);
	g_pWindow = nullptr;
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

bool Init()
{
	// Initialise SDL, report error if it fails
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		Error2MsgBox("SDL Initialisation Failed!\n");
		return false;
	}

	//Initialize PNG/JPG loading 
	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		std::string err_msg = "SDL_image could not initialize!\n";
		err_msg += IMG_GetError();
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", err_msg.c_str(), nullptr);
		return false;
	}

	// Create a window, report error if window not created
	if (!WindowCreate("Test Window",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640, 480,
		SDL_WINDOW_SHOWN))
	{
		Error2MsgBox("Window Creation Failed.\n");
		return false;
	}

	if (!g_Renderer.Create(g_pWindow))
	{
		Error2MsgBox("Renderer Creation Failed.\n");
		return false;
	}

	return true;
}

void Cleanup()
{
	g_Texture.Release();
	g_Renderer.Release();
	WindowRelease();

	// Shutdown SDL
	IMG_Quit();
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
	g_Texture.Render(g_Renderer);

	// Display backbuffer and clear new one ready for next frame
	g_Renderer.Present();
}

void MainLoop()
{
	// Load the image
	g_Texture.CreateFromFile(g_Renderer, "../Gfx/HelloWorld.png");

	g_Running = true;

	// Clear the window
	g_Renderer.ClearBackBuffer();

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