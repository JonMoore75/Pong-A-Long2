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
	if (!m_Ball.CreateTexture(renderer, "..\\gfx\\ball.png"))
		return false;

	m_Ball.SetAnchorPt(GameObject::CENTRE);
	ResetBall();

	FontTTF arialFont;
	if (!arialFont.LoadFont("C:\\Windows\\Fonts\\ARIAL.TTF", 24, SDL_Color{ 0xFF, 0xFF, 0xFF, 0xFF }))
		return false;

	if (!m_textInstruct.CreateTextureFromText(renderer, "Press space to reset ball, Press escape to quit", arialFont))
		return false;

	return true;
}

void PongApp::AppCleanup()
{
}


void PongApp::AppRender(Renderer& renderer)
{
	m_textInstruct.Render(renderer);
	m_Ball.Render(renderer);
}

void PongApp::AppUpdate(double dt)
{
	m_Ball.Update(dt);
}

bool PongApp::OnKeyDown(SDL_Scancode scan, SDL_Keycode key)
{
	Vec2D vel = m_Ball.GetVel();
	switch (key)
	{
	case SDLK_LEFT:
		vel.x = -m_Ball_Speed;
		break;
	case SDLK_RIGHT:
		vel.x = m_Ball_Speed;
		break;
	case SDLK_UP:
		vel.y = -m_Ball_Speed;
		break;
	case SDLK_DOWN:
		vel.y = m_Ball_Speed;
		break;
	}
	m_Ball.SetVelocity(vel);

	return true;
}

bool PongApp::OnKeyUp(SDL_Scancode scan, SDL_Keycode key)
{
	Vec2D vel = m_Ball.GetVel();
	switch (key)
	{
	case SDLK_LEFT:
		if (vel.x < 0.0)
			vel.x = 0.0;
		break;
	case SDLK_RIGHT:
		if (vel.x > 0.0)
			vel.x = 0.0;
		break;
	case SDLK_UP:
		if (vel.y < 0.0)
			vel.y = 0.0;
		break;
	case SDLK_DOWN:
		if (vel.y > 0.0)
			vel.y = 0.0;
		break;
	case SDLK_SPACE:
		ResetBall();
		vel = m_Ball.GetVel();
		break;
	case SDLK_ESCAPE:
		m_Running = false;
		break;
	}
	m_Ball.SetVelocity(vel);

	return true;
}

void PongApp::ResetBall()
{
	m_Ball.SetVelocity(Vec2D());
	m_Ball.SetPosition( Vec2D( m_Window.GetWidth() / 2 ,
						m_Window.GetHeight() / 2  ) );
}