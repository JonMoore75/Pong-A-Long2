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

	m_paddle_max += m_Window.GetHeight();

	if (!m_BounceSound.CreateFromFile("..\\sfx\\Bounce.wav"))
		return false;

	if (!m_ScoreSound.CreateFromFile("..\\sfx\\Score.wav"))
		return false;

	// Ball Creation
	if (!m_Ball.CreateTexture(renderer, "..\\gfx\\ball.png"))
		return false;

	m_Ball.SetAnchorPt(GameObject::CENTRE);
	

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

	UpdateScores();

	ResetBall(LEFT);

	return true;
}

void PongApp::AppCleanup()
{
}


void PongApp::AppRender(Renderer& renderer)
{
	// Set Score text positions
	static int left_x = 4 * m_Window.GetWidth() / 10;
	static int right_x = 6 * m_Window.GetWidth() / 10;

	m_Ball.Render(renderer);
	m_LeftPaddle.Render(renderer);
	m_RightPaddle.Render(renderer);

	m_LeftScoreText.Render(renderer, left_x  - m_LeftScoreText.GetWidth(), 0);
	m_RightScoreText.Render(renderer, right_x, 0);
}

void PongApp::AppUpdate(double dt)
{
	m_Ball.Update(dt);
	MovePaddle(dt, m_LeftPaddle);
	MovePaddle(dt, m_RightPaddle);

	if (CheckForBallPaddleCollision(1, m_LeftPaddle, m_Ball, m_Ball.GetWidth() / 2) ||
		CheckForBallPaddleCollision(-1, m_RightPaddle, m_Ball, m_Ball.GetWidth() / 2))
		m_BounceSound.Play();

	TestForWallCollisions();

	CheckForPointWon();
}


void PongApp::CheckForPointWon()
{
	if (CheckForCircleAxisTrigger(XAXIS, 1, -m_Ball.GetWidth(), m_Ball, m_Ball.GetWidth() / 2))
	{
		m_RightPlayerScore++;
		ResetBall(RIGHT);

		UpdateScores();

		m_ScoreSound.Play();
	}
	else if (CheckForCircleAxisTrigger(XAXIS, -1, m_Window.GetWidth() + m_Ball.GetWidth(), m_Ball, m_Ball.GetWidth() / 2))
	{
		m_LeftPlayerScore++;
		ResetBall(LEFT);

		UpdateScores();

		m_ScoreSound.Play();
	}
}

void PongApp::UpdateScores()
{
	Renderer& renderer = m_Window.GetRenderer();

	FontTTF arialFont;
	if (!arialFont.LoadFont("C:\\Windows\\Fonts\\ARIAL.TTF", 50, SDL_Color{ 0xFF, 0xFF, 0xFF, 0xFF }))
		return;

	m_LeftScoreText.CreateFromText(renderer, std::to_string(m_LeftPlayerScore), arialFont);
	m_RightScoreText.CreateFromText(renderer, std::to_string(m_RightPlayerScore), arialFont);
}

void PongApp::TestForWallCollisions()
{
	 if ( CheckForCircleAxisCollision(YAXIS, 1, 0, m_Ball, m_Ball.GetHeight() / 2) ||
		CheckForCircleAxisCollision(YAXIS, -1, m_Window.GetHeight(), m_Ball, m_Ball.GetHeight() / 2) )
		 m_BounceSound.Play();
}

bool PongApp::CheckForCircleAxisCollision(AXIS axis, int Norm, int planePos, GameObject& circle_obj, double circle_radius)
{
	double& position = (axis == XAXIS) ? circle_obj.GetPos().x : circle_obj.GetPos().y;
	double& velocity = (axis == XAXIS) ? circle_obj.GetVel().x : circle_obj.GetVel().y;

	double dist = Norm*(position - planePos) - circle_radius;

	if (dist < 0.0 && Norm*velocity < 0.0)
	{
		velocity = -velocity;
		position = position - 2 * Norm * dist;

		return true;
	}

	return false;
}

bool PongApp::CheckForCircleAxisTrigger(AXIS axis, int Norm, int planePos, GameObject& circle_obj, double circle_radius)
{
	const double& position = (axis == XAXIS) ? circle_obj.GetPos().x : circle_obj.GetPos().y;
	const double& velocity = (axis == XAXIS) ? circle_obj.GetVel().x : circle_obj.GetVel().y;

	double dist = Norm*(position - planePos) - circle_radius;

	return (dist < 0.0 && Norm*velocity < 0.0);
}

bool PongApp::CheckForBallPaddleCollision(int Norm, GameObject& paddle_obj, GameObject& ball_obj, double circle_radius)
{
	double& position_x = ball_obj.GetPos().x;
	double& velocity_x = ball_obj.GetVel().x;

	double paddle_halfheight = paddle_obj.GetHeight() / 2;
	double paddle_halfwidth = paddle_obj.GetWidth() / 2;

	double planePos = paddle_obj.GetPos().x + Norm*paddle_halfwidth;

	double dist = Norm*(position_x - planePos) - circle_radius;

	// If ball passes through the plane in line with the paddle
	if (dist < 0.0 && Norm*velocity_x < 0.0)
	{
		double timeSinceCollision = abs(dist/velocity_x);

		double relativeYVelocity = ball_obj.GetVel().y - paddle_obj.GetVel().y;

		// Find the balls y position relative to paddle centre at time of collision, scaled to half height of paddle
		double relativeYPosition = (ball_obj.GetPos().y - relativeYVelocity*timeSinceCollision - paddle_obj.GetPos().y)/paddle_halfheight;

		if (relativeYPosition >= -1 && relativeYPosition <= 1)
		{
			velocity_x = -velocity_x;
			position_x = position_x - 2 * Norm * dist;

			// Add a y component depending on position relative to paddle centre
			ball_obj.GetVel().y = relativeYPosition*m_BounceModifier;

			return true;
		}
		// if traj relative to paddle has passed through corner circle

		Vec2D corner_offset(paddle_halfwidth*Norm, paddle_halfheight);
		timeSinceCollision = CheckCornerCollision(paddle_obj.GetPos()+corner_offset, ball_obj.GetPos(), ball_obj.GetVel() - paddle_obj.GetVel(), circle_radius);

		if (timeSinceCollision >= 0.0)
		{
			velocity_x = -velocity_x;
			position_x = position_x - 2 * Norm * timeSinceCollision*velocity_x;

			// Add a y component depending on position relative to paddle centre
			ball_obj.GetVel().y = m_BounceModifier;
		}

		corner_offset = Vec2D(paddle_halfwidth*Norm, -paddle_halfheight);
		timeSinceCollision = CheckCornerCollision(paddle_obj.GetPos() + corner_offset, ball_obj.GetPos(), ball_obj.GetVel() - paddle_obj.GetVel(), circle_radius);

		if (timeSinceCollision >= 0.0)
		{
			velocity_x = -velocity_x;
			position_x = position_x - 2 * Norm * timeSinceCollision*velocity_x;

			// Add a y component depending on position relative to paddle centre
			ball_obj.GetVel().y = -m_BounceModifier;
		}

	}
	return false;
}

double PongApp::CheckCornerCollision(Vec2D& corner_pos, Vec2D& ball_pos, Vec2D& relVelocity, double ball_radius)
{
	Vec2D relPos = ball_pos - corner_pos;
	Vec2D relVel = -relVelocity;

	double A = relVel.dot(relVel);
	double B = 2*relPos.dot(relVel);
	double C = relPos.dot(relPos) - ball_radius*ball_radius;

	double discrim = B*B - 4*A*C;

	// If ball hits corner
	if (discrim >= 0)
		// Take largest root ie biggest time since collision equals earliest collision
		// and return that time since collision
		return (-B + sqrt(discrim)) / (2 * A);
	return -1;
}

void PongApp::MovePaddle(double dt, GameObject& paddle)
{
	paddle.Update(dt);

	double& y = paddle.GetPos().y;

	if (y < m_paddle_min)
		y = m_paddle_min;
	if (y > m_paddle_max)
		y = m_paddle_max;
}

bool PongApp::OnKeyDown(SDL_Scancode scan, SDL_Keycode key)
{
	switch (key)
	{
	case SDLK_w:
		m_LeftPaddle.SetVelocity(Vec2D(0, -m_paddle_speed));
		break;
	case SDLK_s:
		m_LeftPaddle.SetVelocity(Vec2D(0, m_paddle_speed));
		break;
	case  SDLK_UP:
		m_RightPaddle.SetVelocity(Vec2D(0, -m_paddle_speed));
		break;
	case SDLK_DOWN:
		m_RightPaddle.SetVelocity(Vec2D(0, m_paddle_speed));
		break;
	}
	return true;
}

bool PongApp::OnKeyUp(SDL_Scancode scan, SDL_Keycode key)
{
	double& leftVertSpeed = m_LeftPaddle.GetVel().y;
	double& rightVertSpeed = m_RightPaddle.GetVel().y;

	switch (key)
	{
	case SDLK_w:
		if (leftVertSpeed < 0.0)
			leftVertSpeed = 0.0;
		break;
	case SDLK_s:
		if (leftVertSpeed > 0.0)
			leftVertSpeed = 0.0;
		break;
	case  SDLK_UP:
		if (rightVertSpeed < 0.0)
			rightVertSpeed = 0.0;
		break;
	case SDLK_DOWN:
		if (rightVertSpeed > 0.0)
			rightVertSpeed = 0.0;
		break;
	case SDLK_ESCAPE:
		m_Running = false;
		break;
	}

	return true;
}

void PongApp::ResetBall(BALL_DIRN dirn)
{
	double x = m_Window.GetWidth() / 2;
	double y = m_Window.GetHeight() / 2;

	double vx = (dirn == RIGHT) ? m_Ball_XSpeed : -m_Ball_XSpeed;

	m_Ball.SetVelocity( Vec2D(vx, 0) );
	m_Ball.SetPosition( Vec2D(x, y) );
}
