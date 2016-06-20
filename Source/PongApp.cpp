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

	// Ball Creation
	if (!m_Ball.CreateFromFile(renderer, "..\\gfx\\ball.png"))
		return false;

	ResetBall();

	FontTTF arialFont;
	if (arialFont.LoadFont("C:\\Windows\\Fonts\\ARIAL.TTF", 24, SDL_Color{ 0xFF, 0xFF, 0xFF, 0xFF }))
	{
		bool success = m_textInstruct.CreateFromText(renderer, "Press space to reset ball, Press escape to quit", arialFont);
		
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
	m_textInstruct.Render(renderer, 0, 0);
	m_Ball.Render(renderer, m_Ball_Pos.intX(), m_Ball_Pos.intY());
}

void PongApp::AppUpdate(double dt)
{
	m_Ball_Pos += m_Ball_Vel*dt;
}

bool PongApp::OnKeyDown(SDL_Scancode scan, SDL_Keycode key)
{
	switch (key)
	{
	case SDLK_LEFT:
		m_Ball_Vel.x = -m_Ball_Speed;
		break;
	case SDLK_RIGHT:
		m_Ball_Vel.x = m_Ball_Speed;
		break;
	case SDLK_UP:
		m_Ball_Vel.y = -m_Ball_Speed;
		break;
	case SDLK_DOWN:
		m_Ball_Vel.y = m_Ball_Speed;
		break;
	}

	return true;
}

bool PongApp::OnKeyUp(SDL_Scancode scan, SDL_Keycode key)
{
	switch (key)
	{
	case SDLK_LEFT:
		if (m_Ball_Vel.x < 0.0)
			m_Ball_Vel.x = 0.0;
		break;
	case SDLK_RIGHT:
		if (m_Ball_Vel.x > 0.0)
			m_Ball_Vel.x = 0.0;
		break;
	case SDLK_UP:
		if (m_Ball_Vel.y < 0.0)
			m_Ball_Vel.y = 0.0;
		break;
	case SDLK_DOWN:
		if (m_Ball_Vel.y > 0.0)
			m_Ball_Vel.y = 0.0;
		break;
	case SDLK_SPACE:
		ResetBall();
		break;
	case SDLK_ESCAPE:
		m_Running = false;
		break;
	}

	return true;
}

void PongApp::ResetBall()
{
	m_Ball_Vel = Vec2D();

	m_Ball_Pos.x = m_Window.GetWidth() / 2 - m_Ball.GetWidth() / 2;
	m_Ball_Pos.y = m_Window.GetHeight() / 2 - m_Ball.GetHeight() / 2;
}