#include "PongApp.h"

#include <random>
#include <chrono>

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

	TestForWallCollisions();
}


void PongApp::TestForWallCollisions()
{
	CheckForCircleAxisCollision(YAXIS, LESSTHAN, 0, m_Ball, m_Ball.GetHeight() / 2);
	CheckForCircleAxisCollision(YAXIS, GRTERTHAN, m_Window.GetHeight(), m_Ball, m_Ball.GetHeight() / 2);
	CheckForCircleAxisCollision(XAXIS, LESSTHAN, 0, m_Ball, m_Ball.GetWidth() / 2);
	CheckForCircleAxisCollision(XAXIS, GRTERTHAN, m_Window.GetWidth(), m_Ball, m_Ball.GetWidth() / 2);
}

void PongApp::CheckForCircleAxisCollision(AXIS axis, DIRN dirn, int planePos, GameObject& circle_obj, double circle_radius)
{
	double& position = (axis == XAXIS) ? circle_obj.GetPos().x : circle_obj.GetPos().y;
	double& velocity = (axis == XAXIS) ? circle_obj.GetVel().x : circle_obj.GetVel().y;

	int g = (dirn == GRTERTHAN) ? 1 : -1;

	double dist = g*(planePos - position) - circle_radius;

	if (dist < 0.0 && g*velocity > 0.0) 
	{
		velocity = -velocity;
		position = position - 2 * dist;
	}
}

bool PongApp::OnKeyDown(SDL_Scancode scan, SDL_Keycode key)
{
	return true;
}

bool PongApp::OnKeyUp(SDL_Scancode scan, SDL_Keycode key)
{
	switch (key)
	{
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
	static const double angl_conv = 2. * 3.14159265 / 360;
	static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	static std::default_random_engine generator(seed);
	static std::uniform_int_distribution<int> distribution(0, 360);

	double angle = distribution(generator);

	double x = m_Ball_Speed*cos(angle*angl_conv);
	double y = m_Ball_Speed*sin(angle*angl_conv);

	m_Ball.SetVelocity(Vec2D(x, y));
	m_Ball.SetPosition(Vec2D(m_Window.GetWidth() / 2,
		m_Window.GetHeight() / 2));
}
