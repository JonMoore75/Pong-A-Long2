#include "PongApp.h"

PongApp::PongApp(std::string appname) : GameApp(appname)
{

}

PongApp::~PongApp()
{

}

bool PongApp::AppInit()
{
	Renderer& renderer = m_Window.GetRenderer();

	if (!m_Ball.CreateFromFile(renderer, "..\\gfx\\ball.png"))
		return false;

	FontTTF arialFont;
	if (arialFont.LoadFont("C:\\Windows\\Fonts\\ARIAL.TTF", 16, SDL_Color{ 0xFF, 0xFF, 0xFF, 0xFF }))
	{
		bool success = m_textInstruct.CreateFromText(renderer, "Press space to reset ball", arialFont);
		
		return success;
	}
	return false;
}

void PongApp::AppCleanup()
{
	m_Ball.Release();
	m_textInstruct.Release();
}


void PongApp::AppRender(Renderer& renderer)
{
	int y = 0;
	m_textInstruct.Render(renderer, 0, 0);
	m_Ball.Render(renderer, m_Window.GetWidth() / 2, m_Window.GetHeight() / 2);
}

void PongApp::AppUpdate(double dt)
{

}

bool PongApp::OnKeyDown(SDL_Scancode scan, SDL_Keycode key)
{
	return true;
}

bool PongApp::OnKeyUp(SDL_Scancode scan, SDL_Keycode key)
{
	return true;
}
