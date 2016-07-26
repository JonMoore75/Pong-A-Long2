#include "PongApp.h"

#include <random> 

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

	// Dot creation 
	if (!m_TargetDot.CreateTexture(renderer, "..\\gfx\\dot.png"))
		return false;
	m_TargetDot.SetAnchorPt(GameObject::CENTRE);

	int paddle_y = m_Window.GetHeight() / 2;
	int paddle_x = 20;

	// Left paddle creation 
	if (!m_LeftPaddle.CreateTexture(renderer, "..\\gfx\\paddle.png"))
		return false;
	m_LeftPaddle.SetAnchorPt(GameObject::CENTRE);
	m_LeftPaddle.SetPosition(Vec2D(paddle_x, paddle_y));

	// Right paddle creation
	if (!m_RightPaddle.CreateTexture(renderer, "..\\gfx\\paddle.png"))
		return false;
	m_RightPaddle.SetAnchorPt(GameObject::CENTRE);
	m_RightPaddle.SetPosition(Vec2D(m_Window.GetWidth() - paddle_x, paddle_y));

	FontTTF arialFont;
	if (!arialFont.LoadFont("C:\\Windows\\Fonts\\ARIAL.TTF", 24, SDL_Color{ 0xFF, 0xFF, 0xFF, 0xFF }))
		return false;

	if (!m_textInstruct.CreateTextureFromText(renderer, "Press space to reset ball with random direction", arialFont))
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

	m_LeftPaddle.Render(renderer);
	m_RightPaddle.Render(renderer);

	if (m_bShowDot)
		m_TargetDot.Render(renderer);
}

void PongApp::AppUpdate(double dt)
{
	m_bShowDot = false;

	int w = m_Window.GetWidth();
	int h = m_Window.GetHeight();

	double col_dt = dt;

	CheckForCircleLineCollision(col_dt, LineCollider( Vec2D(0, 0), Vec2D(w, 0), Vec2D(0, 1) ), m_Ball, m_Ball.GetHeight() / 2);
	CheckForCircleLineCollision(col_dt, LineCollider( Vec2D(w, 0), Vec2D(0, h), Vec2D(-1, 0) ), m_Ball, m_Ball.GetHeight() / 2);
	CheckForCircleLineCollision(col_dt, LineCollider( Vec2D(w, h), Vec2D(-w, 0), Vec2D(0, -1) ), m_Ball, m_Ball.GetHeight() / 2);
	CheckForCircleLineCollision(col_dt, LineCollider( Vec2D(0, h), Vec2D(0, -h), Vec2D(1, 0) ), m_Ball, m_Ball.GetHeight() / 2);

	m_Ball.Update(dt);

	CheckForCircleAxisCollision(YAXIS, LESSTHAN, 0, m_Ball, m_Ball.GetHeight() / 2);
	CheckForCircleAxisCollision(YAXIS, GRTERTHAN, m_Window.GetHeight(), m_Ball, m_Ball.GetHeight() / 2);

	CheckForCircleAxisCollision(XAXIS, LESSTHAN, 0, m_Ball, m_Ball.GetWidth() / 2);
	CheckForCircleAxisCollision(XAXIS, GRTERTHAN, m_Window.GetWidth(), m_Ball, m_Ball.GetWidth() / 2);
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
	static std::default_random_engine generator;
	static std::uniform_int_distribution<int> distribution(0, 360);

	double angle = distribution(generator);

	double x = m_Ball_Speed*cos(angle*angl_conv);
	double y = m_Ball_Speed*sin(angle*angl_conv);

	m_Ball.SetVelocity( Vec2D(x, y) );
	m_Ball.SetPosition( Vec2D( m_Window.GetWidth() / 2 ,
						m_Window.GetHeight() / 2  ) );
}

void PongApp::CheckForCircleAxisCollision(AXIS axis, DIRN dirn, int planePos, GameObject& circle_obj, double circle_radius)
{
	double& position = (axis == XAXIS) ? circle_obj.GetPos().x : circle_obj.GetPos().y;
	double& velocity = (axis == XAXIS) ? circle_obj.GetVel().x : circle_obj.GetVel().y;

	if (velocity > 0.0 && dirn == GRTERTHAN) // Going down/right
	{
		double edge = position + circle_radius;

		if (edge > planePos)
		{
			position = position - 2 * (edge - planePos);
			velocity = -velocity;
		}
	}
	else if (velocity < 0.0 && dirn == LESSTHAN)// Going up/left
	{
		double edge = position - circle_radius;

		if (edge < planePos)
		{
			position = position + 2 * (planePos - edge);
			velocity = -velocity;
		}
	}
}

void PongApp::CheckForCircleLineCollision(double& dt, LineCollider& line, GameObject& circle_obj, double circle_radius)
{
	Vec2D& circle_vel = circle_obj.GetVel();
	Vec2D B = circle_obj.GetPos() - (line.m_position + line.m_Normal*circle_radius);

	// If moving towards outside of plane
	if (circle_vel.dot(line.m_Normal) < 0.0)
	{
		Vec2D ratio = SolveSimultaneous(line.m_Line.x, -circle_vel.x*dt, line.m_Line.y, -circle_vel.y*dt, B);

		if (ratio.x > 0.0 && ratio.x <= 1.0 && ratio.y > 0.0 /*&& ratio.y <= 1.0*/)
		{
			Vec2D contact_pt = line.m_position + ratio.x*line.m_Line;
			Vec2D collision_pt = contact_pt + line.m_Normal*circle_radius;
			double time2collision = ratio.y*dt;
			double dist2collision = time2collision*m_Ball_Speed;

			m_TargetDot.SetPosition(contact_pt);
			m_bShowDot = true;

			dt = time2collision;
		}
	}
}
